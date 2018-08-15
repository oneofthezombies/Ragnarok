#pragma once

#include <Windows.h>
#include <bitset>

#include "vector2.h"

class Input
{
public:
    static void Reset();
    static bool IsOnceKeyDown(int key);
    static bool IsOnceKeyUp(int key);
    static bool IsStayKeyDown(int key);
    static bool IsToggleKey(int key);
    static Vector2 GetMousePosition();

private:
    static const int _kMaxSizeOfKey = 256;
    static std::bitset<_kMaxSizeOfKey> _key_up;
    static std::bitset<_kMaxSizeOfKey> _key_down;
};
