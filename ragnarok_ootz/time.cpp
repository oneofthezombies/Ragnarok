#include "time.h"

std::chrono::system_clock::time_point Time::_begin_time = std::chrono::system_clock::now();
std::chrono::system_clock::time_point Time::_previous_time = std::chrono::system_clock::now();
float Time::_delta_time = 0.0f;

void Time::Reset()
{
    _begin_time = std::chrono::system_clock::now();
    _previous_time = std::chrono::system_clock::now();
    _delta_time = 0.0f;
}

void Time::Update()
{
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<float> delta_time = current_time - _previous_time;
    _delta_time = delta_time.count();
    _previous_time = current_time;
}

float Time::DeltaTime()
{
    return _delta_time;
}
