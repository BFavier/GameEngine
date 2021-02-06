#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/geometry/vectors/Vector.hpp>

namespace GameEngine
{
    class Vertex;

    class Face
    {
    public:
        Face();
        Face(const Face& other);
        Face(Vertex& v1, Vertex& v2, Vertex& v3);
        Face(Vertex&& v1, Vertex&& v2, Vertex&& v3);
        ~Face();
    public:
        ///< Compute the oriented normal of the face
        Vector normal() const;
        ///< Inverts the direction of the face's normal by swaping two vertices (the 2nd and 3rd).
        void invert_normal();
        ///< Returns true if the two faces coincide (even if they are not ordered in the same way
        static bool coincide(const Face& f1, const Face& f2, double tolerance=1.0E-6);
        ///< Returns true if the three vertex handles are different from nullpointer
        bool valid() const;
    public:
        Face& operator=(const Face& other);
    public:
        std::array<Vertex*, 3> vertices = {nullptr, nullptr, nullptr};
    protected:
        bool _allocated = false;
    };
}
