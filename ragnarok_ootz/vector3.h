#pragma once

struct Vector3
{
    Vector3()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
    {
    }

    Vector3(float const& x, float const& y, float const& z)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    float x;
    float y;
    float z;
};
