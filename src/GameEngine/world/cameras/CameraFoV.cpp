#include <GameEngine/world/cameras/CameraFoV.hpp>
using namespace GameEngine;

CameraFoV::CameraFoV()
{
}

CameraFoV::CameraFoV(const CameraFoV& other) : Camera(other)
{
    *this = other;
}

CameraFoV::CameraFoV(double fov) : Camera()
{
    FoV = fov;
}

CameraFoV::~CameraFoV()
{
}

CameraFoV& CameraFoV::operator=(const CameraFoV& other)
{
    Camera::operator=(other);
    FoV = other.FoV;
    vertical = other.vertical;
    return *this;
}

Matrix CameraFoV::projection(unsigned int image_width, unsigned int image_height) const
{
    if (FoV <= 0.)
    {
        THROW_ERROR("The Field of view of a cameraFoV must be strictly positive")
    }
    double H, W;
    if (vertical)
    {
        H = tan(FoV*Utilities::pi/180 / 2.)*min_distance;
        W = static_cast<double>(image_width)/image_height * H;
    }
    else
    {
        W = tan(FoV*Utilities::pi/180 / 2.)*min_distance;
        H = static_cast<double>(image_height)/image_width * W;
    }
    const double& n = min_distance;
    const double& f = max_distance;
    Matrix project = {{n/W, 0., 0., 0.},
                      {0., n/H, 0., 0.},
                      {0., 0., (n+f)/(n-f), 2*n*f/(n-f)},
                      {0., 0., -1., 0.}};
    return project;
}
