#pragma once
#include "Shader.hpp"
#include <GameEngine/world/cameras/Camera.hpp>
#include <GameEngine/world/Model/Model.hpp>

namespace GameEngine
{
    class Image;

    class ShaderUpdate : public Shader
    {
    public:
        ShaderUpdate();
        ~ShaderUpdate();
    public:
        void draw(const Image& image);
    protected:
        int _index_image = -1;
        Mesh _mesh = {{{0.0, 0.0, 0.0},
                       {1.0, 0.0, 0.0},
                       {0.0, 1.0, 0.0}},
                      {{1.0, 0.0, 0.0},
                       {1.0, 1.0, 0.0},
                       {0.0, 1.0, 0.0}}
                     };
        static const std::string vertex_shader;
        static const std::string fragment_shader;
    };
}
