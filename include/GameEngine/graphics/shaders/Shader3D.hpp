#pragma once
#include "Shader.hpp"
#include <GameEngine/world/cameras/cameras.hpp>
#include <GameEngine/world/Model/Model.hpp>

namespace GameEngine
{
    class Shader3D : public Shader
    {
    public:
        Shader3D();
        ~Shader3D();
    public:
        void draw(const Model& model, const Camera& camera, unsigned int width, unsigned int height, const LayeredData& layers) const;
    protected:
        int _index_to_screen = -1;
        int _index_to_camera = -1;
        int _index_layers = -1;
        int _index_pointer = -1;
        static const std::string vertex_shader;
        static const std::string geometry_shader;
        static const std::string fragment_shader;
    protected:
        void draw_part(const Part& part, const Matrix& base_change, const Matrix& projection, const LayeredData& layers, const void* pointer) const;
    };
}
