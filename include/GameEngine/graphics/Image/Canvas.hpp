#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/graphics/Image/Image.hpp>
#include <GameEngine/graphics/Image/Gbuffer.hpp>

namespace GameEngine
{
    class Canvas : public Image
    {
    public:
        Canvas();
        Canvas(const Canvas& other);
        Canvas(const std::string& path);
        Canvas(unsigned int width, unsigned int height, const RGBA& color={0, 0, 0, 0});
        ~Canvas();
    public:
        void draw();
        void render();
    protected:
        Gbuffer _gbuffer;
    };
}
