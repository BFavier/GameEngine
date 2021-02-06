#include <GameEngine/graphics/RGB.hpp>
using namespace GameEngine;

RGB::RGB()
{
}

RGB::RGB(unsigned char r, unsigned char g, unsigned char b)
{
    R = r;
    G = g;
    B = b;
}

RGB::RGB(const RGB& other)
{
    *this = other;
}

RGB::RGB(const std::initializer_list<unsigned char>& init)
{
    if (init.size() != 3)
    {
        THROW_ERROR("Expected 3 values to define an RGB color, but got "+std::to_string(init.size()))
    }
    std::vector<unsigned char> data = init;
    R = data[0];
    G = data[1];
    B = data[2];
}

RGB::~RGB()
{
}

namespace GameEngine
{
    std::ostream& operator<<(std::ostream& os, const RGB& rgb)
    {
        unsigned int r, g, b;
        r = static_cast<unsigned int>(rgb.R);
        g = static_cast<unsigned int>(rgb.G);
        b = static_cast<unsigned int>(rgb.B);
        os << "(R=" << r << ", G=" << g << ", B=" << b << ")";
        return os;
    }
}

RGB& RGB::operator=(const RGB& other)
{
    R = other.R;
    G = other.G;
    B = other.B;
    return *this;
}

