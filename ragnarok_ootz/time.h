#pragma once

#include <chrono>

class Time
{
public:
    static void Reset();
    static void Update();

    // unit : second
    static float DeltaTime();

private:
    static std::chrono::system_clock::time_point _begin_time;
    static std::chrono::system_clock::time_point _previous_time;
    static float _delta_time;
};
