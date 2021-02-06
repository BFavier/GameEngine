#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/Engine.hpp>
#include <GameEngine/graphics/RGBA.hpp>


namespace GameEngine
{
    class AntiAliasedSurface
    {
    public:
        AntiAliasedSurface();
        AntiAliasedSurface(unsigned int width, unsigned int height, unsigned int AA);
        ~AntiAliasedSurface();
    public:
        void clear(const RGBA& color={0, 0, 0, 0});
        unsigned int FBO();
        void resize(unsigned int width, unsigned int height);
        unsigned int width() const;
        unsigned int height() const;
        unsigned int AA() const;
        void AA(unsigned int samples);
        AntiAliasedSurface& operator=(const AntiAliasedSurface& other);
    protected:
        std::list<std::pair<OpenGLContext, unsigned int>> _FBOs;
        unsigned int _color_buffer = 0;
        unsigned int _depth_buffer = 0;
        unsigned int _width = 1;
        unsigned int _height = 1;
        unsigned int _AA = 1;
    protected:
        void allocate();
        void deallocate();
        unsigned int generate_FBO() const;
    };
}
