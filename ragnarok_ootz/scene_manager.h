#pragma once

#include <deque>

#include "singleton.h"
#include "game_object.h"

#include "input.h"
#include "scene.h"

#include "field_scene.h"
#include "sprite_manager.h"
#include "converter.h"

class _SceneManager : public Singleton<_SceneManager>
{
public:
    _SceneManager()
        : _offset(Vector2())
    {
    }

    virtual ~_SceneManager()
    {
    }

    void Init()
    {
        registerScene(new FieldScene);
        _current_scene = _scenes.front();

        _current_scene->initRecursive();
    }

    void Update()
    {
        _current_scene->updateRecursive();
    }

    void Render(HDC const& hdc)
    {
        _current_scene->renderRecursive(hdc);
    }

    void registerScene(Scene* const& scene)
    {
        _scenes.emplace_back(scene);
    }

    void moveGameObjectToScene(GameObject* go, Scene scene)
    {
    }

    void loadScene(Scene scene)
    {
    }

    Vector2 getCellSizeOfCurrentScene()
    {
        return _current_scene->getCellSize();
    }

    Map& getMapOfCurrentScene() const
    {
        return _current_scene->getMap();
    }

private:
    Vector2 _offset;
    std::deque<Scene*> _scenes;
    Scene* _current_scene;
};

struct SceneManager
{
    _SceneManager* operator()()
    {
        return _SceneManager::getInstance();
    }
};