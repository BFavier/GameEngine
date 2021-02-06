#include <GameEngine/geometry/vectors/Position.hpp>
#include <GameEngine/geometry/vectors/Direction.hpp>
#include <GameEngine/geometry/Referential.hpp>
using namespace GameEngine;

Position::Position()
{
}

Position::Position(const Position& other) : Vector(other)
{
    set_referential(other.referential());
}

Position::Position(const std::initializer_list<double>& elements) : Vector(elements)
{
}

Position::Position(double X, double Y, double Z, const Referential* referential) : Vector(X, Y, Z)
{
    set_referential(referential);
}

Position::Position(const Vector& vector, const Referential* referential) : Vector(vector)
{
    set_referential(referential);
}

Position::~Position()
{
}

glm::vec4 Position::to_vec4() const
{
    return glm::vec4(static_cast<float>(this->x), static_cast<float>(this->y), static_cast<float>(this->z), 1.);
}

Position Position::in(const Referential* referential) const
{
    Position pos(*this);
    pos.to(referential);
    return pos;
}

void Position::to(const Referential* referential)
{
    if (referential == _referential)
    {
        return;
    }
    const Referential* up = _referential;
    const Referential* down = referential;
    std::list<std::pair<Vector, Quaternion>> upward, downward;
    unsigned int d_up = depth();
    unsigned int d_down = 0;
    if (referential != nullptr)
    {
        d_down = referential->position.depth()+1;
    }
    unsigned int d = std::max(d_up, d_down);
    while (up != down)
    {
        if (d == d_up)
        {
            const Referential* new_ref = up->position.referential();
            Quaternion quat = static_cast<Quaternion>(up->orientation.in(new_ref));
            Vector vec = static_cast<Vector>(up->position);
            upward.push_back(std::make_pair(vec, quat));
            up = new_ref;
            d_up--;
        }
        if (d == d_down)
        {
            const Referential* new_ref = down->position.referential();
            Quaternion quat = static_cast<Quaternion>(down->orientation.in(new_ref));
            Vector vec = static_cast<Vector>(down->position);
            downward.push_front(std::make_pair(vec, quat));
            down = new_ref;
            d_down--;
        }
        d--;
    }
    Vector vector = static_cast<Vector>(*this);
    for (std::pair<Vector, Quaternion> transform : upward)
    {
        vector = transform.first + transform.second * vector;
    }
    for (std::pair<Vector, Quaternion> transform : downward)
    {
        vector = transform.second.inverted() * (vector - transform.first);
    }
    x = vector.x;
    y = vector.y;
    z = vector.z;
    set_referential(referential);
}

const Referential* Position::referential() const
{
    return _referential;
}

unsigned int Position::depth() const
{
    if (_referential == nullptr)
    {
        return 0;
    }
    return _referential->position.depth()+1;
}

namespace GameEngine
{
    std::ostream& operator<<(std::ostream& os, const Position& pos)
    {
        os << "Position(" << pos.x << ", " << pos.y << ", " << pos.z;
        if (pos.referential() != nullptr)
        {
            os << "; '" << pos.referential()->name << "'";
        }
        os << ")";
        return os;
    }
}

Position Position::operator-() const
{
    return Position(Vector(-x, -y, -z), _referential);
}

Position& Position::operator=(const Position& other)
{
    Position pos(other.in(_referential));
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

Direction Position::operator-(const Position& other) const
{
    Position coord(other.in(_referential));
    Vector vec(x-coord.x, y-coord.y, z-coord.z);
    return Direction(vec, _referential);
}

Position Position::operator+(const Direction& other) const
{
    Position pos(*this);
    pos += other;
    return pos;
}

Position Position::operator-(const Direction& other) const
{
    Position pos(*this);
    pos -= other;
    return pos;
}

Position& Position::operator+=(const Direction& other)
{
    Direction dir(other.in(_referential));
    x += dir.x;
    y += dir.y;
    z += dir.z;
    return *this;
}

Position& Position::operator-=(const Direction& other)
{
    Direction dir(other.in(_referential));
    x -= dir.x;
    y -= dir.y;
    z -= dir.z;
    return *this;
}

void Position::set_referential(const Referential* referential)
{
    const Referential* other = referential;
    while (other != nullptr)
    {
        if (&(other->position) == this)
        {
            THROW_ERROR("Circular relation of positions");
        }
        other = other->position.referential();
    }
    _referential = referential;
}
