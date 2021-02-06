#include <GameEngine/world/Model/Vertex.hpp>
#include <GameEngine/world/Model/Face.hpp>
using namespace GameEngine;

Vertex::Vertex()
{
}

Vertex::Vertex(const Vertex& other)
{
    *this = other;
}

Vertex::Vertex(const std::initializer_list<double>& elements)
{
    if (elements.size() != 3 && elements.size() != 6)
    {
        THROW_ERROR("Tried to define a vertex with an initializer list a size different from 3 or 6")
    }
    std::vector<double> data = elements;
    position.x = data[0];
    position.y = data[1];
    position.z = data[2];
    if (data.size() >= 6)
    {
        normal.x = data[3];
        normal.y = data[4];
        normal.z = data[5];
    }
}

Vertex::Vertex(double X, double Y, double Z)
{
    position.x = X;
    position.y = Y;
    position.z = Z;
}

Vertex::Vertex(const Vector& pos, const Vector& norm, const RGBA& col)
{
    position = pos;
    normal = norm;
    color = col;
}

Vertex::~Vertex()
{
    //Remove this vertex from the linked faces
    for (Face* face : linked_faces)
    {
        for (Vertex*& v : face->vertices)
        {
            if (v == this)
            {
                v = nullptr;
            }
        }
    }
    delete index;
}

bool Vertex::coincide(const Vertex& v1, const Vertex& v2, double tolerance)
{
    return Vector::equal(v1.position, v2.position, tolerance);
}

GPUvertex Vertex::data() const
{
    GPUvertex vd;
    vd.position = position.to_vec3();
    vd.normal = normal.to_vec3();
    vd.color = {color.R/255., color.G/255., color.B/255., color.A/255.};
    return vd;
}

Vertex& Vertex::operator=(const Vertex& other)
{
    position = other.position;
    normal = other.normal;
    color = other.color;
    return *this;
}
