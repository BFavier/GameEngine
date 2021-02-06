#include <GameEngine/geometry/quaternions/Rotation.hpp>
#include <GameEngine/geometry/quaternions/Orientation.hpp>
#include <GameEngine/geometry/vectors/Direction.hpp>
#include <GameEngine/geometry/Referential.hpp>
using namespace GameEngine;

Rotation::Rotation()
{
}

Rotation::Rotation(const Rotation& other) : Quaternion(other)
{
    _referential = other.referential();
}

Rotation::Rotation(double angle, const Direction& axis)
{
    double radians = angle * Utilities::pi / 180.;
    Vector n = static_cast<Vector>(axis).normed();
    _referential = axis.referential();
    w = cos(radians/2);
    x = n.x * sin(radians/2);
    y = n.y * sin(radians/2);
    z = n.z * sin(radians/2);
}

Rotation::Rotation(const Orientation& from, const Orientation& to, const Referential* referential)
{
    _referential = referential;
    Quaternion quat = static_cast<Quaternion>(to.in(_referential)) * from.in(_referential).inverted();
    Quaternion::operator=(quat);
    normalize();
}

Rotation::Rotation(const Direction& from, const Direction& to, const Referential* referential)
{
    _referential = referential;
    Quaternion quat(from.in(referential), to.in(referential));
    Quaternion::operator=(quat);
}

Rotation::Rotation(const Quaternion& quaternion, const Referential* referential) : Quaternion(quaternion)
{
    _referential = referential;
}

Rotation::~Rotation()
{
}

Direction Rotation::axis() const
{
    return Direction(Vector(x, y, z).normed(), _referential);
}

double Rotation::angle() const
{
    Quaternion n = normed();
    double radians = 2*atan2(Vector(n.x, n.y, n.z).norm(), n.w);
    return radians * 180. / Utilities::pi;
}

Rotation Rotation::in(const Referential* referential) const
{
    Rotation rotation(*this);
    rotation.to(referential);
    return rotation;
}

void Rotation::to(const Referential* referential)
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
    Quaternion::operator=(downward*upward*(*this)*upward.inverted()*downward.inverted());
    _referential = referential;
}

const Referential* Rotation::referential() const
{
    return _referential;
}

namespace GameEngine
{
    std::ostream& operator<<(std::ostream& os, const Rotation& rot)
    {
        os << "Rotation(angle=" << rot.angle() << ", axis=" << static_cast<Vector>(rot.axis());
        if (rot.referential() != nullptr)
        {
            os << "; '" << rot.referential()->name << "'";
        }
        os << ")";
        return os;
    }
}

Rotation& Rotation::operator=(const Rotation& other)
{
    w = other.w;
    x = other.x;
    y = other.y;
    z = other.z;
    _referential = other.referential();
    return *this;
}

Rotation Rotation::operator-() const
{
    return Rotation(inverted(), _referential);
}

Rotation Rotation::operator+(const Rotation& other) const
{
    Rotation rot(*this);
    rot += other;
    return rot;
}

Rotation& Rotation::operator+=(const Rotation& other)
{
    Quaternion& me = static_cast<Quaternion&>(*this);
    Quaternion quat = static_cast<Quaternion>(other.in(_referential)) * me;
    me = quat;
    normalize();
    return *this;
}

Rotation Rotation::operator-(const Rotation& other) const
{
    Rotation rot(*this);
    rot -= other;
    return rot;
}

Rotation& Rotation::operator-=(const Rotation& other)
{
    Quaternion& me = static_cast<Quaternion&>(*this);
    Quaternion quat = other.in(_referential).inverted() * me;
    me = quat;
    normalize();
    return *this;
}

Direction Rotation::operator()(const Direction& other) const
{
    const Referential* referential = other.referential();
    Vector vector = static_cast<Quaternion>(this->in(referential)) * static_cast<Vector>(other);
    return Direction(vector, referential);
}
