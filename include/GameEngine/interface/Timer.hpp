#pragma once
#include <chrono>
#include <GameEngine/Utilities.hpp>

namespace GameEngine
{
    class Timer
    {
    public:
        Timer();
        ~Timer();
        //!< Time in seconds since last reset of dt (or since creation time for the first call).
        double dt(bool reset=true);
        //!< Time in seconds since creation of the timer.
        double t() const;
        ///< Reset the value of dt
        void reset();
        //!< Restarts the timer as if it was just created.
        void restart();
    private:
        std::chrono::time_point<std::chrono::steady_clock> _start;
        std::chrono::time_point<std::chrono::steady_clock> _last;
    };
}
