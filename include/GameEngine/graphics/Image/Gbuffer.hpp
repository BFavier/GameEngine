#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/Engine.hpp>
#include <GameEngine/graphics/RGBA.hpp>
#include <GameEngine/world/cameras/cameras.hpp>
#include <GameEngine/world/Model/Model.hpp>
#include "Image.hpp"

namespace GameEngine
{
    class Gbuffer
    {
    public:
        Gbuffer();
        Gbuffer(unsigned int width, unsigned int height, unsigned int AA);
        ~Gbuffer();
    public:
        void draw(const Image& image);
        void render(const Model& model, const Camera& camera);
        void clear(const RGBA& color={0, 0, 0, 0});
        unsigned int width() const;
        unsigned int height() const;
        void resize(unsigned int width, unsigned int height);
        unsigned int AA() const;
        void AA(unsigned int samples);
        unsigned int transparency_layers() const;
        void transparency_layers(unsigned int number) const;
        Gbuffer& operator=(const Gbuffer& other);
    public:
        unsigned int color_ID() const;
        unsigned int position_ID() const;
        unsigned int normal_ID() const;
        unsigned int pointer_ID() const;
        unsigned int layers_ID() const;
        uint64_t color_handle() const;
        uint64_t position_handle() const;
        uint64_t normal_handle() const;
        uint64_t layers_handle() const;
        unsigned int FBO();
    protected:
        std::list<std::pair<OpenGLContext, unsigned int>> _FBOs;
        unsigned int _width = 0;
        unsigned int _height = 0;
        unsigned int _trasnparency_layers = 0;
        unsigned int _AA = 1;
    protected:
        //Individual buffers of the Gbuffer
        unsigned int _color_ID = 0;
        uint64_t _color_handle = 0;
        unsigned int _position_ID = 0;
        uint64_t _position_ID = 0;
        unsigned int _normal_ID = 0;
        uint64_t _normal_handle = 0;
        unsigned int _material_ID = 0;
        uint64_t _material_handle = 0;
        unsigned int _pointer_ID = 0;
        unsigned int _layers_ID = 0;
        uint64_t _layers_handle = 0;
        unsigned int _depth_buffer = 0;
        unsigned int _stencil_buffer = 0;
    protected:
        void allocate();
        void deallocate();
        unsigned int generate_FBO() const;
    };
}
