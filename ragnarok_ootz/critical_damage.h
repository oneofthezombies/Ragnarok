#pragma once

#include "game_object.h"
#include "transform.h"
#include "animator.h"
#include "sprite_manager.h"
#include "cool_timer.h"
#include "act_manager.h"
#include "game_object_manager.h"

class CriticalDamageEffect : public GameObject
{
public:
    CriticalDamageEffect(Vector2 const& pos, int const damage)
        : _life_timer(CoolTimer(3.0f))
        , _alpha_timer(CoolTimer(0.01f))
        , _alpha(255)
    {
        setLayer(Layer::kEffect);

        _tr = setComponent<Transform>();
        _tr->setPosition(pos);
        _tr->setVelocity(0, -3);

        _at = setComponent<Animator>();
        _at->setActKey(5);
        _at->setSpritesKey(9);

        std::string damage_str = std::to_string(damage);
        for (auto c : damage_str)
            _damage_numbers.emplace_back(c - '0');

        int mid_i = _damage_numbers.size() / 2;
        for (int i = 0; i < static_cast<int>(_damage_numbers.size()); ++i)
            _number_poss.emplace_back(Vector2((i - mid_i) * 13, 5));

        if (_damage_numbers.size() % 2 == 0)
            for (auto& p : _number_poss)
                p.x += 6.5f;
    }

    virtual void update() override
    {
        if (_at->getAnimationKey() == 0 && _at->getFrameKey() == 14)
        {
            _at->setAnimationKey(1);
            _at->setFrameKey(0);
        }

        _tr->move();

        if (_life_timer())
            GameObjectManager::Destroy(getInstanceId());

        if (_alpha_timer())
            if (_alpha > 0)
                _alpha -= 2;
    }

    virtual void render(HDC const& hdc) override
    {
        auto fr = ActManager {}()->getFrame(_at->getActKey(), _at->getAnimationKey(), _at->getActionKey(), _at->getFrameKey());
        for (auto ri : *fr)
            SpriteManager {}()->getSprite(_at->getSpritesKey(), ri->is_mirror, ri->sprite_index)->drawSmart(hdc, _tr->getPosition() + ri->position, Vector2(1.3f, 1.3f), _alpha, ri->rotation);

        for (unsigned int i = 0; i < _damage_numbers.size(); ++i)
            SpriteManager {}()->getSprite(10, false, _damage_numbers[i])->drawSmart(hdc, _tr->getPosition() + fr->front()->position + _number_poss[i], Vector2(1.6f, 1.6f), _alpha, 0);
    }

private:
    Transform* _tr;
    Animator* _at;
    CoolTimer _life_timer;
    CoolTimer _alpha_timer;
    BYTE _alpha;
    std::vector<int> _damage_numbers;
    std::vector<Vector2> _number_poss;
};
