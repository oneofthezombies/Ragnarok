#pragma once

#include "window_manager.h"
#include "game_object.h"
#include "transform.h"
#include "renderer.h"
#include "map.h"

class Scene : public GameObject
{
public:
    Scene()
    {
        _tr = setComponent<Transform>();
        _tr->setPosition(WindowInfo::kWidth / 2, WindowInfo::kHeight / 2);

        _rd = setComponent<Renderer>();
    }

    virtual ~Scene()
    {
    }

    virtual void init()
    {
    }

    virtual void release()
    {
    }

    virtual void update()
    {
    }

    virtual void render(HDC const& hdc)
    {
    }

    virtual Transform* getTransform() const final
    {
        return _tr;
    }

    virtual Renderer* getRenderer() const final
    {
        return _rd;
    }

    virtual Vector2 getCellSize() const final
    {
        return _map.cell_size;
    }

    virtual Map& getMap() final
    {
        return _map;
    }

protected:
    Map _map;

private:
    Transform* _tr;
    Renderer* _rd;
};