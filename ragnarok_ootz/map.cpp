#include "map.h"

const Vector2 Map::tileSize = Vector2(50, 50);

int Map::getAttributes(const Vector2& pos)
{
    if ((static_cast<unsigned int>(pos.y) < size()) && (static_cast<unsigned int>(pos.x) < at(static_cast<unsigned int>(pos.y)).size()))
        return at(static_cast<unsigned int>(pos.y)).at(static_cast<unsigned int>(pos.x));

    assert(false && "Map::getAttributes()");
    return -1;
}
