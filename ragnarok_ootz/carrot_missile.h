#pragma once

#include "game_object.h"
#include "vector2.h"
#include "cool_timer.h"
#include "transform.h"
#include "animator.h"
#include "renderer.h"
#include "game_object_manager.h"
#include "dot_damage.h"
#include "sound_manager.h"
#include "ragnarok_sound.h"

class CarrotMissile : public GameObject
{
public:
    CarrotMissile(Tag const& tag, Vector2 const& position)
        : _life_timer(CoolTimer(2.4f, std::bind(&CarrotMissile::updateLifeTimer, this)))
        , _dot_damage_timer(CoolTimer(999.0f, std::bind(&CarrotMissile::dotDamageTimer, this), 2))
    {
        setLayer(Layer::kEffect);
        setTag(tag);
        _tr = setComponent<Transform>();
        _tr->setPosition(position);

        _at = setComponent<Animator>();
        _at->setActKey(4);
        _at->setSpritesKey(8);

        setComponent<Renderer>();

        SoundManager{}()->play(RagnarokSound::Player::CarrotMissile::Fall{});
    }

    void update() override
    {
        _life_timer();
        _dot_damage_timer();

        if (_at->getFrameKey() == 10) {
            _dot_damage_timer.setCoolTime(0.4f);
            SoundManager{}()->play(RagnarokSound::Player::CarrotMissile::Explosion{});
        }
    }

    void updateLifeTimer()
    {
        GameObjectManager::Destroy(getInstanceId());
    }

    void dotDamageTimer()
    {
        DotDamage* dotDamage = new DotDamage(Tag::kPlayerCarrotMissileDamage, _tr->getPosition(), Vector2(50, 50), 1000 + Random {}()->getIntFromTo(-100, 100), !Utility::IntToBool(Random {}()->getInt(4)));
        GameObjectManager::RegisterGameObject(dotDamage);
    }

private:
    Transform* _tr;
    Animator* _at;
    CoolTimer _life_timer;
    CoolTimer _dot_damage_timer;
};