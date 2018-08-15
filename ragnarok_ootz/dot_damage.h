#pragma once

#include "game_object.h"
#include "vector2.h"
#include "transform.h"
#include "cool_timer.h"
#include "game_object_manager.h"

class DotDamage : public GameObject
{
public:
    DotDamage(const Tag tag, const Vector2& position, const Vector2& size, const int damage, const bool is_critical)
        : _life_timer(CoolTimer(0.1f))
        , _damage(damage)
        , _is_critical(is_critical)
    {
        setTag(tag);

        _tr = setComponent<Transform>();
        _tr->setPosition(position);
        _tr->setSize(size);
    }

    virtual void update() override
    {
        if (_life_timer())
            Destroy(getInstanceId());
    }

    //virtual void render(HDC const& hdc) override
    //{
    //    auto factor = GameObjectManager::GetFactorOfPosition();
    //    auto pos = _tr->getPosition() + factor;
    //    auto size = _tr->getSize();
    //    DrawRectangleCenter(hdc, pos.x, pos.y, size.x, size.y);
    //    std::string pos_str = std::to_string(_tr->getPosition().x) + ", " + std::to_string(_tr->getPosition().y);
    //    TextOut(hdc, pos.x, pos.y, pos_str.c_str(), pos_str.size());
    //}

    int getDamage() const
    {
        return _damage;
    }

    bool isCritical() const
    {
        return _is_critical;
    }

    void setLifeTime(const float time)
    {
        _life_timer.setCoolTime(time);
    }

private:
    Transform* _tr;
    CoolTimer _life_timer;
    int _damage;
    bool _is_critical;
};