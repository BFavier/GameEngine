#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/world/Entity.hpp>
#include <GameEngine/geometry/Matrix.hpp>

namespace GameEngine
{
    class Stand;
    class Model;

    class Camera : public Entity
    {
    public:
        Camera();
        Camera(const Camera& other);
        ~Camera();
    public:
        void detach_stand();
        virtual Matrix base_change(const Model& model) const;
        virtual Matrix projection(unsigned int image_width, unsigned int image_height) const = 0;
    public:
       Camera& operator=(const Camera& other);
    public:
        ///< Distance between focal point and rendering plane. Must be strictly superior to 0.
        double min_distance = 0.01;
        ///< Distance between focal point and far plane (Anything further than the far distance won't be drawn). The higher the distance between rendering plane and far plane, the less precise the depth test will be (Z-fighting can occur).
        double max_distance = 1000;
    protected:
        Stand* _stand = nullptr;
    };
}
