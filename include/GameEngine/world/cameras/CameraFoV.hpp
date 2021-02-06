#pragma once
#include "Camera.hpp"

namespace GameEngine
{
    class CameraFoV : public Camera
    {
    public:
        CameraFoV();
        CameraFoV(const CameraFoV& other);
        CameraFoV(double fov);
        ~CameraFoV();
    public:
        CameraFoV& operator=(const CameraFoV& other);
    public:
        Matrix projection(unsigned int image_width, unsigned int image_height) const;
    public:
        double FoV = 45.;
        bool vertical = true;
    };
}
