#pragma once
#include "Vector.hpp"

namespace GameEngine
{
    class Referential;
    class Position;

    class Direction : public Vector
    {
    public:
        Direction();
        Direction(const Direction& other);
        Direction(const std::initializer_list<double>& elements);
        Direction(double x, double y, double z, const Referential* referential=nullptr);
        Direction(const Vector& vector, const Referential* referential=nullptr);
        ~Direction();
    public:
        glm::vec4 to_vec4() const;
        Direction in(const Referential* referential) const;
        void to(const Referential* referential);
        const Referential* referential() const;
        unsigned int depth() const;
    public:
        friend std::ostream& operator<<(std::ostream& os, const Direction& pos);
        Direction operator-() const;
        Direction& operator=(const Direction& other);
        Direction operator+(const Direction& other) const;
        Direction operator-(const Direction& other) const;
        Direction operator*(double other) const;
        Direction operator/(double other) const;
        friend Direction operator*(double other, const Direction& direction);
        Direction& operator+=(const Direction& other);
        Direction& operator-=(const Direction& other);
        Direction& operator*=(double other);
        Direction& operator/=(double other);
    protected:
        const Referential* _referential = nullptr;
    };
}
