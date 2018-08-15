#pragma once

#include <string>

#include "scene.h"

class FieldScene : public Scene
{
public:
    FieldScene();

    virtual ~FieldScene()
    {
    }

    virtual void init() override
    {
    }

    virtual void release() override
    {
    }

    virtual void update() override;
    virtual void render(HDC const& hdc) override;
};