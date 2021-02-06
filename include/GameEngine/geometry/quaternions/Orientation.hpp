#pragma once
#include "Quaternion.hpp"
#include <GameEngine/geometry/vectors/Direction.hpp>

namespace GameEngine
{
    class Referential;
    class Rotation;

    class Orientation : public Quaternion
    {
    public:
        Orientation();
        Orientation(const Orientation& other);
        Orientation(const Direction& ux, const Direction& uy, const Direction& uz, const Referential* referential=nullptr);
        Orientation(const Quaternion& quaternion, const Referential* referential=nullptr);
        ~Orientation();
    public:
        Orientation in(const Referential* referential) const;
        void to(const Referential* referential);
        const Referential* referential() const;
        static Orientation SLERP(const Orientation& o1, const Orientation& o2, double t);
        unsigned int depth() const;
    public:
        friend std::ostream& operator<<(std::ostream& os, const Orientation& pos);
        Orientation& operator=(const Orientation& other);
        Orientation operator+(const Rotation& rotation) const;
        Orientation& operator+=(const Rotation& rotation);
        Orientation operator-(const Rotation& rotation) const;
        Orientation& operator-=(const Rotation& rotation);
    protected:
        const Referential* _referential = nullptr;
    protected:
        void set_referential(const Referential* referential);
    };
}
