#include <GameEngine/graphics/RGBA.hpp>
using namespace GameEngine;

RGBA::RGBA()
{
}

RGBA::RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : RGB(r, g, b)
{
    A = a;
}

RGBA::RGBA(const RGBA& other) : RGB(other)
{
    A = other.A;
}

RGBA::RGBA(const std::initializer_list<unsigned char>& init)
{
    if (init.size() != 4)
    {
        THROW_ERROR("Expected 4 values to define an RGBA color, but got "+std::to_string(init.size()))
    }
    std::vector<unsigned char> data = init;
    R = data[0];
    G = data[1];
    B = data[2];
    A = data[3];
}

RGBA::~RGBA()
{
}

RGBA RGBA::operator+(const RGBA& other) const
{
    RGBA rgba(*this);
    rgba += other;
    return rgba;
}

RGBA& RGBA::operator+=(const RGBA& other)
{
    double r1 = R/255.;
    double g1 = G/255.;
    double b1 = B/255.;
    double a1 = A/255.;

    double r2 = other.R/255.;
    double g2 = other.G/255.;
    double b2 = other.B/255.;
    double a2 = other.A/255.;

    R = std::round((r1*(1-a2) + r2*a2) * 255.);
    G = std::round((g1*(1-a2) + g2*a2) * 255.);
    B = std::round((b1*(1-a2) + b2*a2) * 255.);
    A = std::round((a1*(1-a2) + a2*a2) * 255.);
    return *this;
}

namespace GameEngine
{
    std::ostream& operator<<(std::ostream& os, const RGBA& rgba)
    {
        unsigned int r, g, b, a;
        r = static_cast<unsigned int>(rgba.R);
        g = static_cast<unsigned int>(rgba.G);
        b = static_cast<unsigned int>(rgba.B);
        a = static_cast<unsigned int>(rgba.A);
        os << "(R=" << r << ", G=" << g << ", B=" << b << ", A=" << a << ")";
        return os;
    }
}

RGBA& RGBA::operator=(const RGBA& other)
{
    RGB::operator=(other);
    A = other.A;
    return *this;
}
