#include <GameEngine/geometry/Referential.hpp>
using namespace GameEngine;

Referential::Referential() : position(Vector(), nullptr), orientation(Quaternion()), ux(Vector(1., 0., 0.), this), uy(Vector(0., 1., 0.), this), uz(Vector(0., 0., 1.), this)
{
}

Referential::Referential(const Referential& other) : position(other.position), orientation(other.orientation), ux(Vector(1., 0., 0.), this), uy(Vector(0., 1., 0.), this), uz(Vector(0., 0., 1.), this)
{
    name = other.name;
}

Referential::~Referential()
{
}

void Referential::attach(Referential& child) const
{
    child.position.to(this);
    child.orientation.to(this);
}

void Referential::detach()
{
    position.to(Referential::absolute);
    orientation.to(Referential::absolute);
}

Referential& Referential::operator=(const Referential& other)
{
    name = other.name;
    position = other.position;
    orientation = other.orientation;
    return *this;
}

const Referential* const Referential::absolute = nullptr;
