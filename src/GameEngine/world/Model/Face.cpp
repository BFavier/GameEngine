#include <GameEngine/world/Model/Face.hpp>
#include <GameEngine/world/Model/Vertex.hpp>
using namespace GameEngine;

Face::Face()
{
    for (Vertex*& v : vertices)
    {
        v = nullptr;
    }
}

Face::Face(const Face& other)
{
    *this = other;
}

Face::Face(Vertex& v1, Vertex& v2, Vertex& v3)
{
    vertices[0] = &v1;
    vertices[1] = &v2;
    vertices[2] = &v3;
    v1.linked_faces.push_back(this);
    v2.linked_faces.push_back(this);
    v3.linked_faces.push_back(this);
}

Face::Face(Vertex&& v1, Vertex&& v2, Vertex&& v3)
{
    vertices[0] = new Vertex(v1);
    vertices[1] = new Vertex(v2);
    vertices[2] = new Vertex(v3);
    _allocated = true;
    vertices[0]->linked_faces.push_back(this);
    vertices[1]->linked_faces.push_back(this);
    vertices[2]->linked_faces.push_back(this);
}

Face::~Face()
{
    for (Vertex* v : vertices)
    {
        //If one of the vertex has been deleted, skip
        if (v == nullptr)
        {
            continue;
        }
        //Remove this face from the linked vertices
        v->linked_faces.remove(this);
        //If the vertex was dynamically allocated, delete it
        if (_allocated)
        {
            delete v;
        }
    }
}

Vector Face::normal() const
{
    Vector l1 = vertices[1]->position - vertices[0]->position;
    Vector l2 = vertices[3]->position - vertices[0]->position;
    return Vector::cross(l1, l2).normed();
}

void Face::invert_normal()
{
    Vertex* v = vertices[3];
    vertices[3] = vertices[2];
    vertices[2] = v;
}

bool Face::coincide(const Face& f1, const Face& f2, double tolerance)
{
    if (not(f1.valid() && f2.valid()))
    {
        return false;
    }
    std::list<Vertex*> vertices;
    for (Vertex* v : f2.vertices)
    {
        vertices.push_back(v);
    }
    unsigned int k = 0;
    for (Vertex* v : f1.vertices)
    {
        std::list<Vertex*>::iterator it;
        for (it = vertices.begin(); it != vertices.end(); it++)
        {
            if (Vertex::coincide(*v, **it, tolerance))
            {
                k++;
                vertices.erase(it);
                break;
            }
        }
    }
    return (k == 3);
}

bool Face::valid() const
{
    return (vertices[0] != nullptr) && (vertices[1] != nullptr) && (vertices[2] != nullptr);
}

Face& Face::operator=(const Face& other)
{
    vertices = other.vertices;
    for (Vertex*& v : vertices)
    {
        if (v == nullptr)
        {
            continue;
        }
        v->linked_faces.push_back(this);
    }
    return *this;
}
