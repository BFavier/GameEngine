#include <GameEngine/world/Model/UV.hpp>
using namespace GameEngine;

UV::UV()
{
}

UV::UV(const UV& other)
{
    u = other.u;
    v = other.v;
}

UV::UV(const std::initializer_list<double>& elements)
{
    if (elements.size() != 2)
    {
        THROW_ERROR("The brace initializer list of an UV object must contain exactly two elements.")
    }
    std::vector<double> coordinates(elements);
    u = coordinates[0];
    v = coordinates[1];
}

UV::UV(double U, double V)
{
    u = U;
    v = V;
}

UV::~UV()
{
}

bool UV::operator==(const UV& other) const
{
    return (u == other.u && v == other.v);
}
