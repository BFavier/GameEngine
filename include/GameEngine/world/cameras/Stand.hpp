#pragma once
#include <GameEngine/world/Entity.hpp>

namespace GameEngine
{
    class Camera;

    class Stand : public Entity
    {
    public:
        ~Stand();
    protected:
        Stand();
        Stand(Camera& camera);
        Stand(const Stand& other) = delete;
    public:
        void attach_camera(Camera& camera);
    public:
        Camera* camera = nullptr;
    };
}
