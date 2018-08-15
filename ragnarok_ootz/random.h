#pragma once

#include <Windows.h>

#include "singleton.h"

class _Random : public Singleton<_Random>
{
public:
    _Random()
    {
        srand(GetTickCount());
    }

    virtual ~_Random()
    {
    }

    // range [from, to]
    int getIntFromTo(int from, int to)
    {
        int range_factor = 0;
        if (from < 0)
            range_factor = from;

        from += range_factor;
        to += range_factor;

        return rand() % (to - from + 1) + from - range_factor;
    }

    // range [0, number)
    int getInt(int number)
    {
        return getIntFromTo(0, number - 1);
    }

    // range [from, to]
    float getFloatFromTo(float from, float to, unsigned number_of_increment = 1000u)
    {
        from *= static_cast<float>(number_of_increment);
        to *= static_cast<float>(number_of_increment);
        return static_cast<float>(getIntFromTo(static_cast<int>(from), static_cast<int>(to))) / static_cast<float>(number_of_increment);
    }

    // range [0.0f, range]
    float getFloat(float range, unsigned number_of_increment = 1000u)
    {
        return getFloatFromTo(0.0f, 3.0f, number_of_increment);
    }
};

struct Random
{
    _Random* operator()()
    {
        return _Random::getInstance();
    }
};
