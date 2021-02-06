#include <GameEngine/world/cameras/Camera.hpp>
#include <GameEngine/world/cameras/Stand.hpp>
#include <GameEngine/world/Model/Model.hpp>
using namespace GameEngine;

Camera::Camera()
{
}

Camera::Camera(const Camera& other) : Entity(other)
{
    *this = other;
}

Camera::~Camera()
{
    detach_stand();
}

Camera& Camera::operator=(const Camera& other)
{
    min_distance = other.min_distance;
    max_distance = other.max_distance;
    return *this;
}

void Camera::detach_stand()
{
    if (_stand != nullptr)
    {
        _stand->camera = nullptr;
    }
    position.to(Referential::absolute);
    orientation.to(Referential::absolute);
}

//Improve me, I am sub-optimal
Matrix Camera::base_change(const Model& model) const
{
    Matrix o(orientation.in(&model).inverted());
    Vector pos = position.in(&model);
    Matrix m1 = {{1., 0., 0., -pos.x},
                 {0., 1., 0., -pos.y},
                 {0., 0., 1., -pos.z},
                 {0., 0., 0., 1.}};
    Matrix m2 = {{o(0,0), o(0,1), o(0,2), 0.},
                 {o(1,0), o(1,1), o(1,2), 0.},
                 {o(2,0), o(2,1), o(2,2), 0.},
                 {0., 0., 0., 1.}};
    Vector vy = upward.normed();
    Vector vz = -forward.normed();
    Vector vx = Vector::cross(vy, vz);
    Matrix m3 = {{vx.x, vx.y, vx.z, 0.},
                 {vy.x, vy.y, vy.z, 0.},
                 {vz.x, vz.y, vz.z, 0.},
                 {0., 0., 0., 1.}};
    return m3*m2*m1;
    /*
    //debug test
    Vector ubis = upward.in(&model);
    Vector fbis = -forward.in(&model);
    Vector sbis = Vector::cross(ubis, fbis).normed();
    Matrix m32 = {{sbis.x, sbis.y, sbis.z, 0.},
                  {ubis.x, ubis.y, ubis.z, 0.},
                  {fbis.x, fbis.y, fbis.z, 0.},
                  {0., 0., 0., 1.}};
    return m32*m1;
    */
}
