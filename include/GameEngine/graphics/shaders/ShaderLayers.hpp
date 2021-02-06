#pragma once
#include "Shader.hpp"
#include <GameEngine/graphics/LayeredData.hpp>

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
        static const std::string vertex_shader;
        static const std::string geometry_shader;
        static const std::string fragment_shader;
    protected:
    };
}

