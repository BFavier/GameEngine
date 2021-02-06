#pragma once
#include "vectors/Direction.hpp"
#include "vectors/Position.hpp"
#include "quaternions/Orientation.hpp"

namespace GameEngine
{
    class Referential
    {
    public:
        Referential();
        Referential(const Referential& other);
        ~Referential();
    public:
        void attach(Referential& child) const;
        void detach();
        Referential& operator=(const Referential& other);
    public:
        std::string name = "referential";
        Position position;
        Orientation orientation;
        const Direction ux;
        const Direction uy;
        const Direction uz;
        static const Referential* const absolute;
    };
}
