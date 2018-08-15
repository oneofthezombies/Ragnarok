#pragma once

#include <Windows.h>

#include "image.h"

class GameManager
{
public:
    static bool Init();
    static bool Reset();
    static void Release();
    static void Update();
    static void Render(HDC const& hdc);
    static void RenderProc(HWND const& hWnd, UINT const& message, UINT const& idEvent, DWORD const& time);

private:
    static Image _back_buffer;
};
