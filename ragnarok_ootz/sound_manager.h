#pragma once

/*

Singleton Pattern

*/

#include <unordered_map>
#include <typeindex>
#include <cassert>
#include <string>

#include <Windows.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "singleton.h"
#include "sound.h"

#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmod64_vc.lib")

class _SoundManager : public Singleton<_SoundManager>
{
public:
    _SoundManager()
        : _system(nullptr)
        , _channel(nullptr)
        , _volume(0.0f)
    {
    }

    virtual ~_SoundManager()
    {
        release();
    }

    void init()
    {
        ERRCHECK(FMOD::System_Create(&_system));
        ERRCHECK(_system->init(2, FMOD_INIT_NORMAL, 0));
    }

    void release()
    {
        for (auto sound : _sounds)
            if (sound.second)
                sound.second->release();

        if (_system)
            _system->release();
    }

    void registerSound(const Sound& sound)
    {
        assert((!sound.isDirectory()) && "SoundManager::registerSound()");

        auto key = std::type_index(typeid(sound));
        auto find = _sounds.find(key);
        if (find == _sounds.end())
            ERRCHECK(_system->createSound(sound.getPath().c_str(), FMOD_DEFAULT, 0, &_sounds[key]));
        else 
            assert(false && "SoundManager::registerSound()");
    }

    void play(const Sound& sound)
    {
        auto find = _sounds.find(std::type_index(typeid(sound)));
        if (find != _sounds.end())
            ERRCHECK(_system->playSound(find->second, NULL, false, &_channel));
        else
            assert(false && "Sound::play()");
    }

    void setVolume(const float volume)
    {
        if (volume < 0.0f)
            _volume = 0.0f;
        else if (volume > 1.0f)
            _volume = 1.0f;
        else
            _volume = volume;
    }

    float getVolume() const
    {
        return _volume;
    }

    void volumeUp(const float volume)
    {
        setVolume(_volume + volume);
    }

    void volumeDown(const float volume)
    {
        setVolume(_volume - volume);
    }

private:
    void ERRCHECK(FMOD_RESULT result)
    {
        if (result != FMOD_OK)
        {
            char str[256];
            sprintf_s(str, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            MessageBox(NULL, str, "TEST", MB_OK);
        }
    }

    FMOD::System* _system;
    FMOD::Channel* _channel;
    std::unordered_map<std::type_index, FMOD::Sound*> _sounds;
    float _volume;
};

struct SoundManager { 
    _SoundManager* operator()() { return _SoundManager::getInstance(); }
};
