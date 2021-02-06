#include <GameEngine/geometry/vectors/Direction.hpp>
#include <GameEngine/geometry/vectors/Position.hpp>
#include <GameEngine/geometry/Referential.hpp>
#include <GameEngine/geometry/quaternions/Rotation.hpp>
using namespace GameEngine;

Direction::Direction() : Vector()
{
}

Direction::Direction(const Direction& other) : Vector(other)
{
    _referential = other.referential();
}

Direction::Direction(const std::initializer_list<double>& elements) : Vector(elements)
{
}

Direction::Direction(double X, double Y, double Z, const Referential* referential) : Vector(X, Y, Z)
{
    _referential = referential;
}

Direction::Direction(const Vector& vector, const Referential* referential) : Vector(vector)
{
    _referential = referential;
}

Direction::~Direction()
{
}

glm::vec4 Direction::to_vec4() const
{
    return glm::vec4(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.);
}

Direction Direction::in(const Referential* referential) const
{
    Direction dir(*this);
    dir.to(referential);
    return dir;
}

void Direction::to(const Referential* referential)
{
    if (referential == _referential)
    {
        return;
    }
    const Referential* up = _referential;
    const Referential* down = referential;
    Quaternion upward, downward;
    unsigned int d_up = depth();
    unsigned int d_down = 0;
    if (referential != nullptr)
    {
        d_down = referential->orientation.depth()+1;
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
    Vector vector = downward * upward * static_cast<Vector>(*this);
    x = vector.x;
    y = vector.y;
    z = vector.z;
    _referential = referential;
}

const Referential* Direction::referential() const
{
    return _referential;
}

unsigned int Direction::depth() const
{
    if (_referential == nullptr)
    {
        return 0;
    }
    return _referential->orientation.depth()+1;
}

namespace GameEngine
{
    std::ostream& operator<<(std::ostream& os, const Direction& dir)
    {
        os << "Direction(" << dir.x << ", " << dir.y << ", " << dir.z;
        if (dir.referential() != nullptr)
        {
            os << "; '" << dir.referential()->name << "'";
        }
        os << ")";
        return os;
    }
}

Direction Direction::operator-() const
{
    return Direction(Vector(-x, -y, -z), _referential);
}

Direction& Direction::operator=(const Direction& other)
{
    Direction dir(other.in(_referential));
    x = dir.x;
    y = dir.y;
    z = dir.z;
    return *this;
}

Direction Direction::operator+(const Direction& other) const
{
    Direction dir(*this);
    dir += other;
    return dir;
}

Direction Direction::operator-(const Direction& other) const
{
    Direction dir(*this);
    dir -= other;
    return dir;
}

Direction Direction::operator*(double other) const
{
    Direction dir(*this);
    dir *= other;
    return dir;
}

Direction Direction::operator/(double other) const
{
    Direction dir(*this);
    dir /= other;
    return dir;
}

namespace GameEngine
{
    Direction operator*(double other, const Direction& direction)
    {
        return direction*other;
    }
}

Direction& Direction::operator+=(const Direction& other)
{
    Direction dir(other.in(_referential));
    x += dir.x;
    y += dir.y;
    z += dir.z;
    return *this;
}

Direction& Direction::operator-=(const Direction& other)
{
    Direction dir(other.in(_referential));
    x -= dir.x;
    y -= dir.y;
    z -= dir.z;
    return *this;
}

Direction& Direction::operator*=(double other)
{
    x *= other;
    y *= other;
    z *= other;
    return *this;
}

Direction& Direction::operator/=(double other)
{
    x /= other;
    y /= other;
    z /= other;
    return *this;
}
