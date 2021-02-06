#pragma once
#include "Shader.hpp"
#include <GameEngine/world/Model/Model.hpp>

namespace GameEngine
{
    class ShaderTest : public Shader
    {
    public:
        ShaderTest();
        ~ShaderTest();
    public:
        void draw();
    protected:
        int _index_image = -1;
        Mesh* _mesh = nullptr;
        Texture* _tex = nullptr;
        Part* _part = nullptr;
        Mesh _other_mesh = {{{0.0, 0.0, 0.0},
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
