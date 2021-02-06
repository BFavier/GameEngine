#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/graphics/RGBA.hpp>

namespace GameEngine
{
    class Material
    {
    public:
        RGBA color = {255, 0, 0, 255};
        RGB color_reflects = {255, 255, 255};
        double shininess = 1.;
        bool two_sided = false;
    };
}
