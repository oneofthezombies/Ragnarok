#pragma once

#include <functional>
#include <Windows.h>
#include <iostream>
#include <string>

struct WindowInfo
{
    static const int kOriginX = 0;
    static const int kOriginY = 0;
    static const int kWidth = 1600;
    static const int kHeight = 900;
    static const int kStyle = WS_OVERLAPPEDWINDOW;
    
    static const int kCellWidth = 50;
    static const int kCellHeight = kCellWidth;
};

struct TimerInfo
{
private:
    enum class Id
    {
        kRender
    };

    enum class Fps
    {
        k60 = 17,
        k30 = 33
    };

public:
    static const Id kId = Id::kRender;
    static const Fps kFps = Fps::k60;
};

class WindowManager
{
public:
    static bool Init(HINSTANCE const& hInstance, 
                     int const& nCmdShow, 
                     std::string const& title, 
                     std::function<void(HWND, UINT, UINT, DWORD)> const& renderProc);
    static void MessageLoop();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
    static void CALLBACK RenderProc(HWND hWnd, UINT message, UINT idEvent, DWORD time);

    static void BindDC(std::function<void(HDC const&)> const& functor);

    static int GetWParamOfMessage();
    static HINSTANCE GetInstance();
    static HWND GetWnd();

private:
    static ATOM MyRegisterClass(HINSTANCE const& hInstance);
    static bool InitInstance(HINSTANCE const& hInstance, int const& nCmdShow);

    static HINSTANCE hInstance;
    static HWND hWnd;
    static std::string title;
    static std::function<void(HWND, UINT, UINT, DWORD)> renderProc;
    static MSG message;
};
