#include <GameEngine/graphics/shaders/Shader3D.hpp>
using namespace GameEngine;

//vertex shader
const std::string Shader3D::vertex_shader =
R"(
#version 450 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
uniform mat4 to_screen;
uniform mat3 to_camera;

out geom_data
{
    uint ID; //Vertex index
    vec4 color; //Vertex color
    vec3 position3D; //Vertex position in camera space
    vec3 normal3D; //Vertex normal in camera space
} data_out;

void main()
{
    //Fragment position in clip space
    gl_Position = to_screen * vec4(position, 1.0);
    //Fragment properties
    data_out.ID = gl_VertexID;
    data_out.color = color;
    data_out.position3D = to_camera * position;
    data_out.normal3D = to_camera * normal;
}
)";

//geometry shader
const std::string Shader3D::geometry_shader =
R"(
#version 450 core
#extension GL_ARB_bindless_texture : require

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in geom_data
{
    uint ID; //Vertex index
    vec4 color; //Vertex color
    vec3 position3D; //Vertex position in camera space
    vec3 normal3D; //Vertex normal in camera space
} data_in[];

out frag_data
{
    vec4 color; //Fragment color
    vec3 position3D; //Fragment position in camera space
    vec3 normal3D; //Fragment normal in camera space
    flat vec3[3] positions3D; //Positions of the triangle vertices in camera space
    flat uint[3] IDs; //Index of the triangle vertices in camera space
} data_out;

void main()
{
    for(uint i=0; i<gl_in.length(); i+=3)
    {
        for (uint j=0; j<3; j++)
        {
            //Vertex positions in clip space
            gl_Position = gl_in[i+j].gl_Position;
            //Vertex parameters
            data_out.color = data_in[i+j].color;
            data_out.position3D = data_in[i+j].position3D;
            data_out.normal3D = data_in[i+j].normal3D;
            //triangle vertices positions
            data_out.positions3D[0] = data_in[i+0].position3D;
            data_out.positions3D[1] = data_in[i+1].position3D;
            data_out.positions3D[2] = data_in[i+2].position3D;
            //triangle vertices ID
            data_out.IDs[0] = data_in[i+0].ID;
            data_out.IDs[1] = data_in[i+1].ID;
            data_out.IDs[2] = data_in[i+2].ID;
            //emitting vertex
            EmitVertex();
        }
        EndPrimitive();
    }
}
)";

//Fragment shader
const std::string Shader3D::fragment_shader =
R"(
#version 450 core
#extension GL_ARB_bindless_texture : require
//#extension GL_ARB_fragment_shader_interlock : require

//Memory lock extension setting
layout(pixel_interlock_unordered) in;

in frag_data
{
    vec4 color; //Fragment color
    vec3 position3D; //Fragment position in camera space
    vec3 normal3D; //Fragment normal in camera space
    flat vec3[3] positions3D; //Positions of the triangle vertices in camera space
    flat uint[3] IDs; //Index of the triangle vertices in camera space
} data;

layout(std430, binding=1) readonly buffer material
{
    vec4 material_color;
    sampler2D textures[];
};

uniform vec2 pointer;
uniform layout(r32f) image3D layers;

out vec4 fragment_color;

//Surface of a triangle (p1, p2, p3)
float surface(in vec3 p1, in vec3 p2, in vec3 p3)
{
    return 0.5*length(cross(p2-p1, p3-p1));
}

//return the weights of each vertices (p1, p2, p3) associated to the fragment p
vec3 vertex_weights(in vec3 p1, in vec3 p2, in vec3 p3, in vec3 p)
{
    float area = surface(p1, p2, p3);
    return vec3(surface(p, p2, p3)/area, surface(p1, p, p3)/area, surface(p1, p2, p)/area);
}

//Reads the UV coordinate of the vertex of given ID from a texture storage
vec2 get_vertex_uv(in sampler2D tex, in uint ID)
{
    ivec2 size = textureSize(tex, 0);
    if (ID > size.x)
    {
        return vec2(0., 0.);
    }
    float u = texelFetch(tex, ivec2(ID, 0), 0).r;
    float v = texelFetch(tex, ivec2(ID, 1), 0).r;
    return vec2(u, v);
}

//Interpolation on 3 vertices of the fragment UV. Vertex UVs are stored in a texture 'tex'
vec2 get_fragment_uv(in sampler2D tex, in uint[3] IDs, in vec3 weights)
{
    //Get the UV of each vertex
    vec2 uv1 = get_vertex_uv(tex, IDs[0]);
    vec2 uv2 = get_vertex_uv(tex, IDs[1]);
    vec2 uv3 = get_vertex_uv(tex, IDs[2]);
    //Calculate the UV of the fragment
    return weights.x*uv1 + weights.y*uv2 + weights.z*uv3;
}

//Paint a color on top of another one
vec4 paint(in vec4 color_bottom, in vec4 color_top)
{
    float top = color_top.a;
    float bot = (1-color_top.a)*color_bottom.a;
    float alpha = top+bot;
    if (alpha < 1.0E-6)
    {
        return vec4(0., 0., 0., 0.);
    }
    return vec4(color_bottom.xyz*bot/alpha + color_top.xyz*top/alpha, alpha);
}

//fill layers
/*
void fill_layers(in vec4 color, in vec3 position, in vec3 normal, in vec2 pointer, in vec2 index)
{
    //If the current fragment doesn't have transparency, save normal and position
    if (color.a >= 0.99)
    {
        imageStore(layers, ivec3(index, 0), vec4(pointer.x));
        imageStore(layers, ivec3(index, 1), vec4(pointer.y));
        imageStore(layers, ivec3(index, 2), vec4(position.x));
        imageStore(layers, ivec3(index, 3), vec4(position.y));
        imageStore(layers, ivec3(index, 4), vec4(position.z));
        imageStore(layers, ivec3(index, 5), vec4(normal.x));
        imageStore(layers, ivec3(index, 6), vec4(normal.y));
        imageStore(layers, ivec3(index, 7), vec4(normal.z));
    }
    else
    {
        imageStore(layers, ivec3(index, 0), vec4(pointer.x));
        imageStore(layers, ivec3(index, 1), vec4(pointer.y));
        imageStore(layers, ivec3(index, 2), vec4(position.x));
        imageStore(layers, ivec3(index, 3), vec4(position.y));
        imageStore(layers, ivec3(index, 4), vec4(position.z));
        imageStore(layers, ivec3(index, 5), vec4(normal.x));
        imageStore(layers, ivec3(index, 6), vec4(normal.y));
        imageStore(layers, ivec3(index, 7), vec4(normal.z));
    }
}
*/

void main()
{
    //Compute the weight of each vertex of the triangular surface
    vec3 weights = vertex_weights(data.positions3D[0], data.positions3D[1], data.positions3D[2], data.position3D);

    //Apply material color on top of vertex color
    //fragment_color = paint(data.color, material_color);

    //Applying textures
    for (uint i=0; i<textures.length(); i+=2)
    {
        vec2 uv = get_fragment_uv(textures[i], data.IDs, weights);
        vec4 texture_color = texture(textures[i+1], uv);
        fragment_color = paint(fragment_color, texture_color);
        fragment_color = texture_color;

        if (texture_color.r > 1. || texture_color.g > 1. || texture_color.b > 1.)
        {
            fragment_color = vec4(1., 0., 1., 1.);
        }
    }

    //skip empty fragments
    /*
    if (length(fragment_color) < 1.0E-3)
    {
        discard;
    }
    */

    //Transparency layers
    /*
    ivec2 index = ivec2(floor(gl_FragCoord.xy));
    vec3 dimensions = imageSize(layers);
    beginInvocationInterlockARB();
    fill_layers(data.color, data.position3D, data.normal3D, pointer, index);
    endInvocationInterlockARB();
    */

    //Shading
    /*
    float cos_angle = 1.;
    if (length(data.normal3D) > 1.0E-3)
    {
        cos_angle = dot(data.normal3D, vec3(0., 0., 1.)) / length(data.normal3D);
    }
    fragment_color = vec4(fragment_color.xyz * (0.5+0.5*cos_angle), fragment_color.w);
    */
}
)";

Shader3D::Shader3D() : Shader(Shader3D::vertex_shader, Shader3D::geometry_shader, Shader3D::fragment_shader)
{
    _index_to_screen = glGetUniformLocation(_program_ID, "to_screen");
    _index_to_camera = glGetUniformLocation(_program_ID, "to_camera");
    _index_layers = glGetUniformLocation(_program_ID, "layers");
    _index_pointer = glGetUniformLocation(_program_ID, "pointer");
}
Shader3D::~Shader3D()
{
}

void Shader3D::draw(const Model& model, const Camera& camera, unsigned int width, unsigned int height, const LayeredData& layers) const
{
    Vector vec = camera.upward.in(nullptr);
    glUseProgram(_program_ID);
    Matrix base_change = camera.base_change(model);
    Matrix projection = camera.projection(width, height);
    for (const Part& part : model.parts)
    {
        draw_part(part, base_change, projection, layers, &model);
    }
}

void Shader3D::draw_part(const Part& part, const Matrix& base_change, const Matrix& projection, const LayeredData& layers, const void* pointer) const
{
    glUseProgram(_program_ID);
    if (part.material.two_sided)
    {
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glEnable(GL_CULL_FACE);
    }
    glBindVertexArray(part.get_mesh()->VAO());
    glm::mat4 to_screen = (projection*base_change).to_mat4();
    glUniformMatrix4fv(_index_to_screen, 1, GL_FALSE, &to_screen[0][0]);
    glm::mat3 to_camera = base_change.to_mat3();
    glUniformMatrix3fv(_index_to_camera, 1, GL_FALSE, &to_camera[0][0]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, part.ID());
    glUniformHandleui64ARB(_index_layers, layers.handle());
    float pointer_data[2];
    memcpy(pointer_data, pointer, sizeof(void*));
    glUniform2f(_index_pointer, pointer_data[0], pointer_data[1]);
    Utilities::check_GL_error();
    glDrawElements(GL_TRIANGLES, 3*part.get_mesh()->faces.size(), GL_UNSIGNED_INT, 0);
    Utilities::check_GL_error();
}
