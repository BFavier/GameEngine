#pragma once
#include "Quaternion.hpp"

namespace GameEngine
{
    class Orientation;
    class Direction;

    class Rotation : public Quaternion
    {
    public:
        Rotation();
        Rotation(const Rotation& other);
        Rotation(double angle, const Direction& axis);
        Rotation(const Orientation& from, const Orientation& to, const Referential* referential=nullptr);
        Rotation(const Direction& from, const Direction& to, const Referential* referential=nullptr);
        Rotation(const Quaternion& quaternion, const Referential* referential=nullptr);
        ~Rotation();
    public:
        Direction axis() const;
        double angle() const;
        Rotation in(const Referential* referential) const;
        void to(const Referential* referential);
        const Referential* referential() const;
    public:
        friend std::ostream& operator<<(std::ostream& os, const Rotation& rot);
        Rotation& operator=(const Rotation& other);
        Rotation operator-() const;
        Rotation operator+(const Rotation& other) const;
        Rotation& operator+=(const Rotation& other);
        Rotation operator-(const Rotation& other) const;
        Rotation& operator-=(const Rotation& other);
        Direction operator()(const Direction& other) const;
    protected:
        const Referential* _referential = nullptr;
    };
}
