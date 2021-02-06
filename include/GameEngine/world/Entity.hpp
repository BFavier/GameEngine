#pragma once
#include <GameEngine/geometry/geometry.hpp>

namespace GameEngine
{
    class Entity : public Referential
    {
    public:
        Entity();
        Entity(const Entity& other);
        ~Entity();
    public:
        Entity& operator=(const Entity& other);
        void orient(const Direction& sideward, const Direction& forward, const Direction& upward);
        void orient(const Entity& other);
    public:
        Direction forward;
        Direction upward;
        Direction sideward;
    };
}
