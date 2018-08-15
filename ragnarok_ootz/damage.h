#pragma once

#include <string>

#include "game_object.h"
#include "transform.h"
#include "animator.h"
#include "renderer.h"
#include "cool_timer.h"
#include "utility.h"
#include "game_object_manager.h"
#include "sprite_manager.h"

class DamageEffect : public GameObject
{
public:
    DamageEffect(Vector2 const& pos, int const damage)
        : _life_timer(CoolTimer(1.15f))
        , _alpha_timer(CoolTimer(0.01f))
        , _alpha(255)
        , _velocity_y_factor(1.7f)
    {
        setLayer(Layer::kEffect);

        _tr = setComponent<Transform>();
        _tr->setPosition(pos);
        _tr->setVelocity(5.0f, -25.0f);

        _damage_numbers = Utility::GetNumberVectorFromNumber(damage);
        int mid_i = _damage_numbers.size() / 2;
        for (int i = 0; i < static_cast<int>(_damage_numbers.size()); ++i)
            _number_poss.emplace_back(Vector2((i - mid_i) * 13, 5));

        if (_damage_numbers.size() % 2 == 0)
            for (auto& p : _number_poss)
                p.x += 6.5f;
    }

    virtual void update() override
    {
        auto previous_velocity = _tr->getVelocity();
        _tr->setVelocity(previous_velocity.x, previous_velocity.y + _velocity_y_factor);
        _tr->move();

        if (_life_timer())
            GameObjectManager::Destroy(getInstanceId());

        if (_alpha_timer())
            if (_alpha > 0)
                _alpha -= 5;
    }

    virtual void render(HDC const& hdc) override
    {
        for (unsigned int i = 0; i < _damage_numbers.size(); ++i)
            SpriteManager{}()->getSprite(10, false, _damage_numbers[i])->drawSmart(hdc, _tr->getPosition() + _number_poss[i], Vector2(1.6f, 1.6f), _alpha, 0);
    }

private:
    Transform* _tr;
    CoolTimer _life_timer;
    CoolTimer _alpha_timer;
    BYTE _alpha;
    std::vector<int> _damage_numbers;
    std::vector<Vector2> _number_poss;
    const float _velocity_y_factor;
};