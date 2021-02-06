#pragma once
#include <GameEngine/Utilities.hpp>

namespace GameEngine
{
    class UV
    {
    public:
        UV();
        UV(const UV& other);
        UV(const std::initializer_list<double>& elements);
        UV(double u, double v);
        ~UV();
    public:
        double u = 0.;
        double v = 0.;
    public:
        bool operator==(const UV& other) const;
    };
}
