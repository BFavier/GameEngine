#pragma once
#include <GameEngine/Utilities.hpp>

namespace GameEngine
{
    //Describes the status of a button
    struct Button
    {
        bool down = false;//True if the key is pressed
        bool changed = false;//True if the status changed at the last update
    };
}
