#include "game_manager.h"

#include "act_manager.h"
#include "sprite_manager.h"
#include "scene_manager.h"
#include "game_object_manager.h"
#include "window_manager.h"
#include "time.h"
#include "sound_manager.h"

Image GameManager::_back_buffer = Image();

bool GameManager::Init()
{
    if (!ActManager{}()->load())
        return false;

    if (!SpriteManager{}()->load())
        return false;

	AddFontResourceA("./malgun.ttf");

    SoundManager {}()->init();

    SceneManager {}()->Init();
    GameObjectManager::Init();

    return Reset();
}

bool GameManager::Reset()
{
    Time::Reset();
    Input::Reset();

    return true;
}

void GameManager::Release()
{
	RemoveFontResourceA("./malgun.ttf");
    SoundManager {}()->release();
}

void GameManager::Update()
{
    Time::Update();
    SceneManager {}()->Update();
    GameObjectManager::Update();
}

void GameManager::Render(HDC const& hdc)
{
    SceneManager {}()->Render(hdc);
    GameObjectManager::Render(hdc);
}

void GameManager::RenderProc(HWND const& hWnd, UINT const& message, UINT const& idEvent, DWORD const& time)
{
    Update();

    _back_buffer.drawBlack();
    SetBkMode(_back_buffer.getDC(), TRANSPARENT);
    //SetBkMode(_back_buffer.getDC(), OPAQUE);

    Render(_back_buffer.getDC());

    WindowManager::BindDC([](HDC const& hdc) { _back_buffer.draw(hdc, Vector2(WindowInfo::kWidth / 2.0f, WindowInfo::kHeight / 2.0f)); });
}
