#pragma once

#include <Windows.h>

#include "component.h"
#include "render_info.h"

class Renderer : public Component
{
public:
    Renderer(GameObject* const& owner);

    void render(HDC const& hdc);

    void setSpritesKey(int const& key);
    int getSpritesKey() const;
    void setRenderInfo(RenderInfo const& render_info);
    RenderInfo getRenderInfo() const;
    RenderInfo* accessRenderInfo();

private:
    int _sprites_key;
    RenderInfo _render_info;
};