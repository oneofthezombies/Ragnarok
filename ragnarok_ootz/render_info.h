#pragma once

#include "vector2.h"

struct RenderInfo
{
    RenderInfo()
        : sprite_index(0)
        , position(Vector2())
        , is_mirror(false)
        , alpha(255)
        , scale(Vector2(1, 1))
        , rotation(0.0f)
    {
    }

    int sprite_index;
    Vector2 position;
    bool is_mirror;
    BYTE alpha;
    Vector2 scale;
    float rotation;
};
