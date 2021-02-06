#include <GameEngine/geometry/quaternions/Orientation.hpp>
#include <GameEngine/geometry/Referential.hpp>
#include <GameEngine/geometry/quaternions/Rotation.hpp>
using namespace GameEngine;

Orientation::Orientation()
{
}

Orientation::Orientation(const Orientation& other) : Quaternion(other)
{
    set_referential(other.referential());
}

//https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
Orientation::Orientation(const Direction& ux, const Direction& uy, const Direction& uz, const Referential* referential)
{
    Vector vx, vy, vz;
    double max_norm = std::max(std::max(ux.norm(), uy.norm()), uz.norm());
    if ((uy.norm() > 1.0E-9*max_norm) && (uz.norm() > 1.0E-9*max_norm))
    {
        vy = uy.in(referential).normed();
        vz = uz.in(referential);
        vx = Vector::cross(vy, vz).normed();
        vz = Vector::cross(vx, vy).normed();
    }
    else if ((ux.norm() > 1.0E-9*max_norm) && (uz.norm() > 1.0E-9*max_norm))
    {
        vx = ux.in(referential).normed();
        vz = uz.in(referential);
        vy = Vector::cross(vz, vx).normed();
        vz = Vector::cross(vx, vy).normed();
    }
    else if ((ux.norm() > 1.0E-9*max_norm) && (uy.norm() > 1.0E-9*max_norm))
    {
        vx = ux.in(referential).normed();
        vy = uy.in(referential);
        vz = Vector::cross(vx, vy).normed();
        vy = Vector::cross(vz, vx).normed();
    }
    else
    {
        THROW_ERROR("At least two non-null vectors must be given to the constructor")
    }
    double tr = vx.x + vy.y + vz.z;
    if (tr > 0)
    {
      double S = sqrt(tr+1.0) * 2;
      w = 0.25 * S;
      x = (vy.z - vz.y) / S;
      y = (vz.x - vx.z) / S;
      z = (vx.y - vy.x) / S;
    }
    else if ((vx.x > vy.y) && (vx.x > vz.z))
    {
      double S = sqrt(1.0 + vx.x - vy.y - vz.z) * 2;
      w = (vy.z - vz.y) / S;
      x = 0.25 * S;
      y = (vy.x + vx.y) / S;
      z = (vz.x + vx.z) / S;
    }
    else if (vy.y > vz.z)
    {
      double S = sqrt(1.0 + vy.y - vx.x - vz.z) * 2;
      w = (vz.x - vx.z) / S;
      x = (vy.x + vx.y) / S;
      y = 0.25 * S;
      z = (vz.y + vy.z) / S;
    }
    else
    {
      double S = sqrt(1.0 + vz.z - vx.x - vy.y) * 2;
      w = (vx.y - vy.x) / S;
      x = (vz.x + vx.z) / S;
      y = (vz.y + vy.z) / S;
      z = 0.25 * S;
    }
    _referential = referential;
}

Orientation::Orientation(const Quaternion& quaternion, const Referential* referential) : Quaternion(quaternion)
{
    set_referential(referential);
    normalize();
}

Orientation::~Orientation()
{
}

Orientation Orientation::in(const Referential* referential) const
{
    Orientation orientation(*this);
    orientation.to(referential);
    return orientation;
}

void Orientation::to(const Referential* referential)
{
    if (referential == _referential)
    {
        return;
    }
    const Referential* up = _referential;
    const Referential* down = referential;
    Quaternion upward, downward;
    unsigned int d_up = 0;
    if (up != nullptr)
    {
        d_up = up->orientation.depth()+1;
    }
    unsigned int d_down = 0;
    if (down != nullptr)
    {
        d_down = down->orientation.depth()+1;
    }
    unsigned int d = std::max(d_up, d_down);
    while (up != down)
    {
        if (d == d_up)
        {
            upward = static_cast<Quaternion>(up->orientation)*upward;
            up = up->orientation.referential();
            d_up--;
        }
        if (d == d_down)
        {
            downward = downward*static_cast<Quaternion>(down->orientation).inverted();
            down = down->orientation.referential();
            d_down--;
        }
        d--;
    }
    Quaternion::operator=(downward*upward*(*this));
    set_referential(referential);
}

const Referential* Orientation::referential() const
{
    return _referential;
}

Orientation Orientation::SLERP(const Orientation& o1, const Orientation& o2, double t)
{
    const Referential* referential = o1.referential();
    Quaternion q1(o1);
    Quaternion q2(o2.in(referential));
    return Orientation(Quaternion::SLERP(q1, q2, t), referential);
}

unsigned int Orientation::depth() const
{
    if (_referential == nullptr)
    {
        return 0;
    }
    return _referential->orientation.depth() + 1;
}

namespace GameEngine
{
    std::ostream& operator<<(std::ostream& os, const Orientation& orient)
    {
        Quaternion quat = static_cast<Quaternion>(orient);
        Vector ux = quat * Vector(1., 0., 0.);
        Vector uy = quat * Vector(0., 1., 0.);
        Vector uz = quat * Vector(0., 0., 1.);
        os << "Orientation(ux=" << ux << ", uy=" << uy << ", uz=" << uz;
        if (orient.referential() != nullptr)
        {
            os << "; '" << orient.referential()->name << "'";
        }
        os << ")";
        return os;
    }
}

Orientation& Orientation::operator=(const Orientation& other)
{
    Orientation orient(other);
    orient.to(_referential);
    w = orient.w;
    x = orient.x;
    y = orient.y;
    z = orient.z;
    return *this;
}

Orientation Orientation::operator+(const Rotation& rotation) const
{
    Orientation orient(*this);
    orient += rotation;
    return orient;
}

Orientation& Orientation::operator+=(const Rotation& rotation)
{
    Quaternion& me = static_cast<Quaternion&>(*this);
    Quaternion quat = static_cast<Quaternion>(rotation.in(_referential)) * me;
    me = quat;
    normalize();
    return *this;
}

Orientation Orientation::operator-(const Rotation& rotation) const
{
    Orientation orient(*this);
    orient -= rotation;
    return orient;
}

Orientation& Orientation::operator-=(const Rotation& rotation)
{
    Quaternion& me = static_cast<Quaternion&>(*this);
    Quaternion quat = rotation.in(_referential).inverted() * me;
    me = quat;
    normalize();
    return *this;
}

void Orientation::set_referential(const Referential* referential)
{
    const Referential* other = referential;
    while (other != nullptr)
    {
        if (&(other->orientation) == this)
        {
            THROW_ERROR("Circular relation of orientations");
        }
        other = other->orientation.referential();
    }
    _referential = referential;
}
