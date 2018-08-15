#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "map.h"

class Converter
{
public:
    static Vector2 Math2DToDirect2D(Vector2 const& from)
    {
        return Vector2(from.x, -from.y);
    }

    static Vector2 Direct2DToMath2D(Vector2 const& from)
    {
        return Math2DToDirect2D(from);
    }

    static Vector3 Direct2DToDirect3D(Vector2 const& from)
    {
        return Vector3(from.x, from.y, 0.0f);
    }

    static std::pair<int, int> WorldSpaceToMapSpace(Vector2 const& position, Vector2 const& cell_size)
    {
        return std::make_pair(static_cast<int>(position.x / cell_size.x), static_cast<int>(position.y / cell_size.y));
    }

    static Vector2 MapSpaceToWorldSpace(std::pair<int, int> const& cell, Vector2 const& cell_size)
    {
        return Vector2((static_cast<float>(cell.first) + 0.5f) * cell_size.x, (static_cast<float>(cell.second) + 0.5f) * cell_size.y);
    }
};

struct CoordinateSystem
{
    static Vector2 Math2DToDirect2D(const Vector2& coord)
    {
        return Vector2(coord.x, -coord.y);
    }

    static Vector2 Direct2DToMath2D(const Vector2& coord)
    {
        return Math2DToDirect2D(coord);
    }
};

struct Space
{
    static Vector2 WorldToMap(const Vector2& pos)
    {
        return Vector2(static_cast<int>(pos.x / Map::tileSize.x), 
                       static_cast<int>(pos.y / Map::tileSize.y));
    }

    static Vector2 MapToWorld(const Vector2& pos)
    {
        return Vector2((static_cast<float>(pos.x) + 0.5f) * Map::tileSize.x, 
                       (static_cast<float>(pos.y) + 0.5f) * Map::tileSize.y);
    }
};
