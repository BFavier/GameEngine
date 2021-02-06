#include <GameEngine/graphics/shaders/ShaderTest.hpp>
using namespace GameEngine;

//vertex shader
const std::string ShaderTest::vertex_shader =
R"(
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;

out frag_data
{
    vec2 uv;
} data_out;

void main()
{
    gl_Position = vec4(position*2. - 1., 1.);
    data_out.uv = position.xy;
}
)";

/*
//Fragment shader
const std::string ShaderTest::fragment_shader =
R"(
#version 450 core

in frag_data
{
    vec2 uv;
} data;

out vec4 fragment_color;

void main()
{
    fragment_color = vec4(1., 0., 1., 1.);
}
)";
*/

//Fragment shader
const std::string ShaderTest::fragment_shader =
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
    fragment_color = vec4(data.uv.x, data.uv.y, 0., 1.);
}
)";

/*
layout(std430, binding=1) readonly buffer material
{
    vec4 material_color;
    sampler2D textures[];
};
*/

/*
for (uint i=0; i<textures.length(); i+=2)
{
    vec4 texture_color = texture(textures[i+1], data.uv);
    fragment_color = texture_color;
}
*/

ShaderTest::ShaderTest() : Shader(ShaderTest::vertex_shader, ShaderTest::fragment_shader)
{
    _index_image = glGetUniformLocation(_program_ID, "image");
    _mesh = new Mesh({{{0.0, 0.0, 0.0},
                            {1.0, 0.0, 0.0},
                            {0.0, 1.0, 0.0}},
                            {{1.0, 0.0, 0.0},
                            {1.0, 1.0, 0.0},
                            {0.0, 1.0, 0.0}}
                            });
    Image* image = new Image("images/smile.png");
    image->n_users = 0;
    UVmap* uvmap = new UVmap({{0., 0.}, {1., 0.}, {1., 1.}, {0., 1.}});
    uvmap->n_users = 0;
    _tex = new Texture(image, uvmap);
    _part = new Part(_mesh, Material(), {*_tex});
}

ShaderTest::~ShaderTest()
{
    delete _part;
    delete _tex;
    delete _mesh;
}

/*
void ShaderTest::draw()
{
    glUseProgram(_program_ID);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(_other_mesh.VAO());
    //glBindVertexArray(_mesh->VAO());
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _part->ID());
    glDrawElements(GL_TRIANGLES, _other_mesh->faces.size(), GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
}
*/

void ShaderTest::draw()
{
    glUseProgram(_program_ID);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(_other_mesh.VAO());
    glUniformHandleui64ARB(_index_image, _tex->get_image()->handle());
    glDrawElements(GL_TRIANGLES, _other_mesh.faces.size()*3, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
}
