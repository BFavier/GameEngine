#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/geometry/vectors/Vector.hpp>
#include <GameEngine/graphics/RGBA.hpp>

namespace GameEngine
{
    class Face;

    ///< data structure representing a vertex in GPU memory
    struct GPUvertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 color;
    };

    ///< A Vertex is a point of a mesh
    class Vertex
    {
    public:
        Vertex();
        Vertex(const Vertex& other);
        Vertex(const std::initializer_list<double>& elements);
        Vertex(double X, double Y, double Z);
        Vertex(const Vector& position, const Vector& normal={0., 0., 0.}, const RGBA& color={255, 255, 255, 255});
        ~Vertex();
    public:
        static bool coincide(const Vertex& v1, const Vertex& v2, double tolerance=1.0E-6);
        GPUvertex data() const;
    public:
        Vertex& operator=(const Vertex& other);
    public:
        Vector position;
        Vector normal;
        RGBA color = {0, 0, 0, 0};
        std::list<Face*> linked_faces;
        unsigned int* index = new unsigned int;
    };
}
