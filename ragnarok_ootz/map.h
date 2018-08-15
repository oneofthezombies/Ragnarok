#pragma once

#include <vector>

#include "vector2.h"

struct Map : public std::vector<std::vector<int>>
{
    int getAttributes(const Vector2& pos);

    Vector2 cell_size;

    static const Vector2 tileSize;
};
