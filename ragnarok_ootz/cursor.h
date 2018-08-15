#pragma once

#include "game_object.h"
#include "transform.h"
#include "animator.h"
#include "renderer.h"

#include "scene_manager.h"

class Cursor : public GameObject
{
public:
    Cursor()
    {
        _tr = setComponent<Transform>();
        //_at = setComponent<Animator>();
        _rd = setComponent<Renderer>();
        _rd->setSpritesKey(5);
        _rd->accessRenderInfo()->alpha = 100;
    }

    virtual void update() override
    {
        auto cell_size = SceneManager {}()->getCellSizeOfCurrentScene();
        _tr->setPosition(Converter::MapSpaceToWorldSpace(Converter::WorldSpaceToMapSpace(Input::GetMousePosition(), cell_size), cell_size));
    }

    virtual void render(HDC const& hdc) override
    {
        //std::string position_str;
        //position_str = "mouse position : (" + std::to_string(static_cast<int>(_tr->getPosition().x)) + ", " + std::to_string(static_cast<int>(_tr->getPosition().y)) + ")";
        //TextOut(hdc, 0, 0, position_str.c_str(), position_str.size());
    }

private:
    Transform* _tr;
    Animator* _at;
    Renderer* _rd;
};