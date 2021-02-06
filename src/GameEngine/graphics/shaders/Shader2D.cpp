#include <GameEngine/graphics/shaders/Shader2D.hpp>
#include <GameEngine/graphics/Image/Image.hpp>
using namespace GameEngine;

//vertex shader
const std::string Shader2D::vertex_shader =
R"(
#version 450 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;

out frag_data
{
    vec2 uv;
} data_out;

void main()
{
    //passing data
    gl_Position = vec4(position*2. - 1., 1.);
    data_out.uv = position.xy;
}
)";

//Fragment shader
const std::string Shader2D::fragment_shader =
R"(
#version 450 core
#extension GL_ARB_bindless_texture : require

in frag_data
{
    vec2 uv;
} data;

uniform sampler2D image;

out vec4 fragment_color;

void main()
{
    fragment_color = texture(image, data.uv);
}
)";

Shader2D::Shader2D() : Shader(Shader2D::vertex_shader, Shader2D::fragment_shader)
{
    _index_image = glGetUniformLocation(_program_ID, "image");
}

Shader2D::~Shader2D()
{
}

void Shader2D::draw(const Image& image)
{
    image.update();
    glUseProgram(_program_ID);
    glDepthFunc(GL_ALWAYS);
    glBindVertexArray(_mesh.VAO());
    glUniformHandleui64ARB(_index_image, image.handle());
    glDrawElements(GL_TRIANGLES, 3*_mesh.faces.size(), GL_UNSIGNED_INT, 0);
    glDepthFunc(GL_LESS);
}
