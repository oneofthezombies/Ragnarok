#include <Windows.h>

#include "game_manager.h"
#include "window_manager.h"

//#include <fmod.hpp>
//#include <fmod_errors.h>

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(lib, "msimg32.lib")

//#pragma comment(lib, "fmod_vc.lib")

//FMOD::System *systemS = NULL;
//FMOD::Sound *sound = NULL;
//FMOD::Sound *sound1 = NULL;
//FMOD::Channel *channel = NULL;
//
//void ERRCHECK(FMOD_RESULT result)
//{
//    if (result != FMOD_OK)
//    {
//        char str[256];
//        sprintf_s(str, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
//        MessageBox(NULL, str, "TEST", MB_OK);
//    }
//}
//
//void DemoSound()
//{
//    FMOD_RESULT result;
//
//    result = FMOD::System_Create(&systemS);  // Create the main system object.
//    ERRCHECK(result);
//
//    result = systemS->init(2, FMOD_INIT_NORMAL, 0); // Initialize FMOD.
//    ERRCHECK(result);
//
//    // 사운드로딩
//    result = systemS->createSound("resources/sounds/40.mp3", FMOD_DEFAULT, 0, &sound);  // FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
//    ERRCHECK(result);
//
//    result = systemS->createSound("resources/sounds/frus_die.wav", FMOD_DEFAULT, 0, &sound1);  // FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
//    ERRCHECK(result);
//}
//
//void PLAYsound()
//{
//    FMOD_RESULT result;
//    result = systemS->playSound(sound, NULL, false, &channel);
//
//    channel->setVolume(0.5f);
//
//    ERRCHECK(result);
//}
//
//void PLAYsound1()
//{
//    FMOD_RESULT result;
//    result = systemS->playSound(sound1, NULL, false, &channel);
//
//    channel->setVolume(0.5f);
//
//    ERRCHECK(result);
//}
//
//void DeleteSound()
//{
//    if (sound) sound->release();
//    if (sound1) sound1->release();
//    if (systemS) systemS->release();
//
//}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszCmdParam,
                     int nCmdShow)
{
    if (!GameManager::Init())
        return 0;

    if (!WindowManager::Init(hInstance, nCmdShow, std::string("Ragnarok"), GameManager::RenderProc))
        return 0;
    //DemoSound();
    //PLAYsound();

    //PLAYsound1();
    WindowManager::MessageLoop();
    GameManager::Release();
    //DeleteSound();
    return WindowManager::GetWParamOfMessage();
}

/*

collision

AABB
사각충돌 괜춘 하지만 로테이트된 도형에선 오류있다

OBB


*/

/*

게임 오브젝트의 부모가 존재하지 않는다면 월드 좌표이다.
게임 오브젝트의 부모가 존재한다면 부모가 원점인 로컬 좌표이다.

*/