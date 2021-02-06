#include <GameEngine/world/cameras/Stand.hpp>
#include <GameEngine/world/cameras/Camera.hpp>
using namespace GameEngine;

Stand::Stand()
{
    name="stand";
}

Stand::Stand(Camera& cam)
{
    name="stand";
    attach_camera(cam);
}

Stand::~Stand()
{
    if (camera != nullptr)
    {
        camera->detach_stand();
    }
}

void Stand::attach_camera(Camera& cam)
{
    if (camera != nullptr)
    {
        camera->detach();
    }
    camera = &cam;
    cam.position.to(this);
    cam.position = Vector(0., 0., 0.);
    cam.orientation.to(this);
    cam.orientation = Quaternion();
}
