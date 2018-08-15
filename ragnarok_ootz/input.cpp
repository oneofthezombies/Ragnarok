#include "input.h"

#include "window_manager.h"
#include "game_object_manager.h"

std::bitset<Input::_kMaxSizeOfKey> Input::_key_up;
std::bitset<Input::_kMaxSizeOfKey> Input::_key_down;

void Input::Reset()
{
    _key_up.reset();
    _key_down.reset();
}

bool Input::IsOnceKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        if (!_key_down[key])
        {
            _key_down.set(key);
            return true;
        }
    }
    else
        _key_down.reset(key);

    return false;
}

bool Input::IsOnceKeyUp(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
        _key_up.set(key);
    else
        if (_key_up[key])
        {
            _key_up.reset(key);
            return true;
        }

    return false;
}

bool Input::IsStayKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
        return true;

    return false;
}

bool Input::IsToggleKey(int key)
{
    if (GetKeyState(key) & 0x0001)
        return true;

    return false;
}

Vector2 Input::GetMousePosition()
{
    POINT pos;
    GetCursorPos(&pos);
    ScreenToClient(WindowManager::GetWnd(), &pos);

    return Vector2(static_cast<float>(pos.x), static_cast<float>(pos.y)) - GameObjectManager::GetFactorOfPosition();
}
