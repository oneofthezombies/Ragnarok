#include "window_manager.h"

HINSTANCE WindowManager::hInstance = nullptr;
HWND WindowManager::hWnd = nullptr;
MSG WindowManager::message = MSG {};
std::string WindowManager::title = "";
std::function<void(HWND, UINT, UINT, DWORD)> WindowManager::renderProc = std::function<void(HWND, UINT, UINT, DWORD)> {};

ATOM WindowManager::MyRegisterClass(HINSTANCE const& hInstance)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = title.c_str();
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    return RegisterClass(&wc);
}

bool WindowManager::InitInstance(HINSTANCE const& hInstance, int const& nCmdShow)
{
    hWnd = CreateWindow(title.c_str(), title.c_str(), WindowInfo::kStyle,
                        WindowInfo::kOriginX, WindowInfo::kOriginY,
                        WindowInfo::kWidth, WindowInfo::kHeight,
                        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return false;

    RECT rc { 0, 0, WindowInfo::kWidth, WindowInfo::kHeight };
    AdjustWindowRect(&rc, WindowInfo::kStyle, false);
    SetWindowPos(hWnd, NULL,
                 WindowInfo::kOriginX, WindowInfo::kOriginY,
                 WindowInfo::kWidth, WindowInfo::kHeight, 
                 SWP_NOZORDER | SWP_NOMOVE);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return true;
}

bool WindowManager::Init(HINSTANCE const& hInstance, 
                         int const& nCmdShow, 
                         std::string const& title, 
                         std::function<void(HWND, UINT, UINT, DWORD)> const& renderProc)
{
    WindowManager::hInstance = hInstance;
    WindowManager::title = title;
    WindowManager::renderProc = renderProc;
    
    MyRegisterClass(hInstance);
    return InitInstance(hInstance, nCmdShow);
}

void WindowManager::MessageLoop()
{
    while (GetMessage(&message, 0, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int WindowManager::GetWParamOfMessage()
{
    return static_cast<int>(message.wParam);
}

LRESULT CALLBACK WindowManager::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd,
                 static_cast<UINT_PTR>(TimerInfo::kId),
                 static_cast<UINT>(TimerInfo::kFps),
                 (TIMERPROC)RenderProc);
    }
    break;
    case WM_DESTROY:
    {
        KillTimer(hWnd,
                  static_cast<UINT_PTR>(TimerInfo::kId));
        PostQuitMessage(0);
        return 0;
    }
    break;
    }

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void CALLBACK WindowManager::RenderProc(HWND hWnd, UINT message, UINT idEvent, DWORD time)
{
    if (renderProc)
        renderProc(hWnd, message, idEvent, time);
}

HINSTANCE WindowManager::GetInstance()
{
    return hInstance;
}

HWND WindowManager::GetWnd()
{
    return hWnd;
}

void WindowManager::BindDC(std::function<void(HDC const&)> const& functor)
{
    HDC hdc = GetDC(hWnd);

    functor(hdc);

    ReleaseDC(hWnd, hdc);
}
