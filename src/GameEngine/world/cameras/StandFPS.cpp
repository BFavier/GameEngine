#include <GameEngine/world/cameras/StandFPS.hpp>
#include <GameEngine/world/cameras/Camera.hpp>
using namespace GameEngine;

StandFPS::StandFPS()
{
}

StandFPS::StandFPS(Camera& cam) : Stand(cam)
{
}

StandFPS::~StandFPS()
{
}

void StandFPS::yaw(double angle)
{
    orientation += Rotation(angle, upward);
}

void StandFPS::pitch(double angle)
{
    double p = _pitch + angle;
    p = std::max(-90., std::min(p, 90.));
    set_pitch(p);
}

double StandFPS::get_pitch() const
{
    return _pitch;
}

void StandFPS::set_pitch(double angle)
{
    _pitch = angle;
    camera->orientation = Orientation(Quaternion(_pitch, sideward), this);
}
