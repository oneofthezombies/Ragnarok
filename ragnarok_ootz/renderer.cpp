#include "renderer.h"

#include "game_object.h"
#include "transform.h"
#include "animator.h"

#include "act_manager.h"
#include "sprite_manager.h"

Renderer::Renderer(GameObject* const& owner)
    : Component(owner)
{
}

void Renderer::render(HDC const& hdc)
{
    if (auto tr = getOwner()->getComponent<Transform>())
    {
        auto owner_pos = tr->getPositionOfWorldSpace();
        if (auto at = getOwner()->getComponent<Animator>())
        {
            auto fr = ActManager {}()->getFrame(at->getActKey(), at->getAnimationKey(), at->getActionKey(), at->getFrameKey());
            for (auto ri : *fr)
                SpriteManager {}()->getSprite(at->getSpritesKey(), ri->is_mirror, ri->sprite_index)->drawSmart(hdc, owner_pos + DotProduct(_render_info.scale, ri->position), DotProduct(_render_info.scale, ri->scale), ri->alpha, ri->rotation);
        }
        else
            SpriteManager {}()->getSprite(_sprites_key, _render_info.is_mirror, _render_info.sprite_index)->drawSmart(hdc, owner_pos + _render_info.position, _render_info.scale, _render_info.alpha, _render_info.rotation);
    }
}

void Renderer::setSpritesKey(int const& key)
{
    _sprites_key = key;
}

int Renderer::getSpritesKey() const
{
    return _sprites_key;
}

void Renderer::setRenderInfo(RenderInfo const& render_info)
{
    _render_info = render_info;
}

RenderInfo Renderer::getRenderInfo() const
{
    return _render_info;
}

RenderInfo* Renderer::accessRenderInfo()
{
    return &_render_info;
}
