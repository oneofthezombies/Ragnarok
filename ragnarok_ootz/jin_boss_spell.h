#pragma once

#include "vector2.h"
#include "game_object.h"
#include "cool_timer.h"
#include "transform.h"
#include "animator.h"
#include "renderer.h"
#include "dot_damage.h"

class JinBossSpell : public GameObject
{
public:
    JinBossSpell(const Tag tag, const Vector2& pos)
        : _life_timer(CoolTimer(3.0f, std::bind(&JinBossSpell::updateLifeTimer, this)))
        , _dot_damage_timer(CoolTimer(0.2f, std::bind(&JinBossSpell::dotDamageTimer, this)))
    {
        setTag(tag);
        setLayer(Layer::kShadow);

        _tr = setComponent<Transform>();
        _tr->setPosition(pos);
        
        _at = setComponent<Animator>();
        _at->setActKey(6);
        _at->setSpritesKey(21);

        setComponent<Renderer>();

        //Sound play
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
        // 데미지 바꿔야함
        DotDamage* dotDamage = new DotDamage(Tag::kBossSpellDamage, _tr->getPosition(), Vector2::one * 200, 9999, false);
        RegisterGameObject(dotDamage);
    }

private:
    Transform* _tr;
    Animator* _at;
    CoolTimer _life_timer;
    CoolTimer _dot_damage_timer;
};