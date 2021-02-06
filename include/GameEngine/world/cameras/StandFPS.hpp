#pragma once
#include "Stand.hpp"

namespace GameEngine
{
    class StandFPS : public Stand
    {
    public:
        StandFPS();
        StandFPS(Camera& camera);
        ~StandFPS();
    public:
        ///< Rotate the stand around its upward direction
        void yaw(double angle);
        ///< increment the pitch of the camera and clip it in the [-90, 90]° range
        void pitch(double angle);
        ///< Returns the pitch of the camera
        double get_pitch() const;
        ///< Set the pitch of the camera
        void set_pitch(double angle);
    protected:
        double _pitch = 0.;
    };
}
