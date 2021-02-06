#include <GameEngine/world/Entity.hpp>
using namespace GameEngine;

Entity::Entity() : forward(uy), upward(uz), sideward(ux)
{
}

Entity::Entity(const Entity& other) : Referential(other), forward(other.forward), upward(other.upward), sideward(other.sideward)
{
}

Entity::~Entity()
{
}

Entity& Entity::operator=(const Entity& other)
{
    Referential::operator=(other);
    forward = static_cast<Vector>(other.forward);
    upward = static_cast<Vector>(other.upward);
    sideward = static_cast<Vector>(other.sideward);
    return *this;
}

void Entity::orient(const Direction& s, const Direction& f, const Direction& u)
{
    Orientation to(s, f, u, this);
    Orientation from(sideward, forward, upward, this);
    orientation += Rotation(from, to, this);
}

void Entity::orient(const Entity& other)
{
    orient(other.sideward, other.forward, other.upward);
}
