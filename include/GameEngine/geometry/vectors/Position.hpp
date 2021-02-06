#pragma once
#include "Vector.hpp"

namespace GameEngine
{
    class Referential;
    class Direction;

    class Position : public Vector
    {
    public:
        Position();
        Position(const Position& other);
        Position(const std::initializer_list<double>& elements);
        Position(double x, double y, double z, const Referential* referential=nullptr);
        Position(const Vector& vector, const Referential* referential=nullptr);
        ~Position();
    public:
        glm::vec4 to_vec4() const;
        Position in(const Referential* referential) const;
        void to(const Referential* referential);
        const Referential* referential() const;
        unsigned int depth() const;
    public:
        friend std::ostream& operator<<(std::ostream& os, const Position& pos);
        Position operator-() const;
        Position& operator=(const Position& other);
        Position operator+(const Direction& other) const;
        Position& operator+=(const Direction& other);
        Position operator-(const Direction& other) const;
        Position& operator-=(const Direction& other);
        Direction operator-(const Position& other) const;
    protected:
        const Referential* _referential = nullptr;
    protected:
        void set_referential(const Referential* referential);
    };
}
