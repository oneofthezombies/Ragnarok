#pragma once

#include <functional>

#include "game_object.h"
#include "vector2.h"
#include "transform.h"
#include "animator.h"
#include "renderer.h"
#include "cool_timer.h"
#include "game_object_manager.h"
#include "dot_damage.h"
#include "random.h"
#include "scene_manager.h"
#include "sound_manager.h"
#include "ragnarok_sound.h"

class LightningSpell : public GameObject
{
public:
    LightningSpell(Tag const& tag, Vector2 const& position)
        : _life_timer(CoolTimer(2.0f, std::bind(&LightningSpell::updateLifeTimer, this)))
        , _dot_damage_timer(CoolTimer(0.4f, std::bind(&LightningSpell::dotDamageTimer, this), 4))
    {
        setTag(tag);
        setLayer(Layer::kEffect);

        _tr = setComponent<Transform>();
        _tr->setPosition(position);

        _at = setComponent<Animator>();
        _at->setActKey(3);
        _at->setSpritesKey(7);

        setComponent<Renderer>()->accessRenderInfo()->scale = Vector2(1.1f, 1.1f);

        SoundManager{}()->play(RagnarokSound::Player::Lightning::Storm{});
    }

    void update() override
    {
        _life_timer();
        _dot_damage_timer();
    }

    void updateLifeTimer()
    {
        Destroy(getInstanceId());
    }

    void dotDamageTimer()
    {
        DotDamage* dotDamage = new DotDamage(Tag::kPlayerLightningDamage, _tr->getPosition(), Vector2(50, 50), 500 + Random {}()->getIntFromTo(-50, 50), !Utility::IntToBool(Random {}()->getInt(4)));
        RegisterGameObject(dotDamage);
    }

private:
    Transform* _tr;
    Animator* _at;
    CoolTimer _life_timer;
    CoolTimer _dot_damage_timer;
};