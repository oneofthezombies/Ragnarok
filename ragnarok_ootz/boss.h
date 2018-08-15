#pragma once

#include "game_object.h"
#include "transform.h"
#include "animator.h"
#include "renderer.h"

#include "cool_timer.h"
#include "a_star.h"
#include "random.h"
#include "critical_damage.h"
#include "scene_manager.h"
#include "damage.h"
#include "dot_damage.h"
#include "sound_manager.h"
#include "ragnarok_sound.h"

enum class BossState
{
    kIdle = 0,
    kMove = 1,
    kAttack = 2,
    kReceivingDamage = 3,
    kDie = 4
};

class Boss : public GameObject
{
public:
    Boss()
        : GameObject()
        , _move_timer(2.0f, std::bind(&Boss::move, this))
        , _move_step_timer(0.3f, std::bind(&Boss::moveStep, this))
        , _hp(5000)
    {
        setTag(Tag::kBoss);
        _state = BossState::kIdle;

        _tr = setComponent<Transform>();
        _tr->setPosition(Space::MapToWorld(Vector2(47, 5)));
        _tr->setSize(150, 100);

        _at = setComponent<Animator>();
        _at->setActKey(2);
        _at->setSpritesKey(4);
        _at->setAnimationKey(static_cast<int>(_state));

        setComponent<Renderer>();

        GameObject* shadow = new GameObject;
        shadow->setLayer(Layer::kShadow);
        auto shadow_tr = shadow->setComponent<Transform>();
        shadow_tr->setPosition(0, -20);
        auto shadow_rd = shadow->setComponent<Renderer>();
        shadow_rd->setSpritesKey(3);
        auto shadow_ri = shadow_rd->accessRenderInfo();
        shadow_ri->alpha = 100;
        shadow_ri->scale = Vector2(3.5f, 3.5f);
        registerChild(shadow);
    }

    virtual void update() override
    {
        switch (_state)
        {
        case BossState::kIdle:
        case BossState::kMove:
        {
            _move_timer();
            _move_step_timer();
            _tr->move();
        }
        break;
        case BossState::kAttack:
        {
            if (_at->getFrameKey() == 5)
            {
                DotDamage* dd= new DotDamage(Tag::kBossAttack, _tr->getPosition() + Space::MapToWorld(Math::DirectionToUnitVector2(_tr->getDirection()) * 2),
                                             Vector2(150, 150), 2000 + Random{}()->getIntFromTo(-100, 100), !Utility::IntToBool(Random{}()->getInt(4)));
                RegisterGameObject(dd);
                _at->setFrameKey(6);
            }
            else if (_at->getFrameKey() == 12)
                _state = BossState::kIdle;
        }
        break;
        case BossState::kReceivingDamage:
        {
            if (_at->getFrameKey() == 5)
                if (_hp <= 0)
                {
                    _state = BossState::kDie;
                    SoundManager{}()->play(RagnarokSound::Boss::Die{});
                }
                else
                    _state = BossState::kIdle;
        }
        break;
        case BossState::kDie:
        {
            if (_at->getFrameKey() == 31)
                Destroy(getInstanceId());
        }
        break;
        }

        _at->setAnimationKey(static_cast<int>(_state));
    }

    virtual void render(HDC const& hdc) override {
        for (auto cell : _road)
            SpriteManager {}()->getSprite(6, false, 0)->drawSmart(hdc, Space::MapToWorld(Vector2::From(cell)), Vector2::one, 60, 0.0f);

        auto pos = Vector2(WindowInfo::kWidth, WindowInfo::kHeight) * 0.5f - GameObjectManager::GetFactorOfPosition() + Vector2(550, -250) + (_tr->getPosition() - Vector2(2900, 2550) * 0.5f) * 0.125;
        SpriteManager{}()->getSprite(19)->drawSmart(hdc, pos, Vector2::one * 0.5f, 200);
        //std::string str = std::to_string(pos);
        //TextOut(hdc, 500, 0, str.c_str(), str.size());
    }

    virtual void onCollisionEnter(GameObject* other, RECT const& area) override
    {
        switch (other->getTag()) {
        case Tag::kPlayerLightningDamage: 
        case Tag::kPlayerCarrotMissileDamage: {
            switch (_state) {
            case BossState::kIdle:
            case BossState::kMove:
            case BossState::kAttack:
            case BossState::kReceivingDamage: {
                _state = BossState::kReceivingDamage;

                DotDamage* damage_go = static_cast<DotDamage*>(other);
                int damage = damage_go->getDamage();
                _hp -= damage;

                if (damage_go->isCritical())
                    RegisterGameObject(new CriticalDamageEffect(_tr->getPosition() + Vector2(0, -100), damage * 2));
                else
                    RegisterGameObject(new DamageEffect(_tr->getPosition() + Vector2(0, -100), damage));

                Destroy(other->getInstanceId());

                SoundManager{}()->play(RagnarokSound::Boss::Hit{});

                switch (other->getTag()) {
                case Tag::kPlayerLightningDamage:
                    //SoundManager{}()->play(RagnarokSound::Player::Lightning::Hit{});
                    break;
                case Tag::kPlayerCarrotMissileDamage:
                    SoundManager{}()->play(RagnarokSound::Player::CarrotMissile::Hit{});
                    break;
                }

            }
            break;
            }
        }
        break;
        case Tag::kPlayerIAm:
        {
            switch (_state)
            {
            case BossState::kIdle:
            case BossState::kMove:
            {
                _state = BossState::kMove;

                _road.clear();
                _road = AStar {}(SceneManager {}()->getMapOfCurrentScene(), 
                                 Space::WorldToMap(_tr->getPosition()).toIntPair(), 
                                 Space::WorldToMap(other->getComponent<Transform>()->getPosition()).toIntPair());
            }
            break;
            }
        }
        break;
        case Tag::kPlayerCollider:
        {
            switch (_state)
            {
            case BossState::kIdle:
            case BossState::kMove:
            {
                _state = BossState::kAttack;
                _tr->setDirection(Math::GetDirectionFromTo(_tr->getPosition(), other->getComponent<Transform>()->getPosition()));
                SoundManager{}()->play(RagnarokSound::Boss::Attack{});
            }
            break;
            }
        }
        break;
        }
    }

    void move()
    {
        if (_road.empty())
        {
            _state = BossState::kMove;

            auto current = Space::WorldToMap(_tr->getPosition());
            auto next = Vector2(static_cast<int>(current.x) + Random {}()->getIntFromTo(-2, 2), 
                                static_cast<int>(current.y) + Random {}()->getIntFromTo(-2, 2));
            auto map = SceneManager {}()->getMapOfCurrentScene();
            if (!map[static_cast<int>(next.y)][static_cast<int>(next.x)])
                _road = AStar {}(map, current.toIntPair(), next.toIntPair());
            _move_timer.setCoolTime(Random {}()->getFloatFromTo(1.5f, 3.0f));

            //SoundManager{}()->play(RagnarokSound::Boss::Move{});
        }
    }

    void moveStep()
    {
        if (!_road.empty())
        {
            auto next = Space::MapToWorld(Vector2::From(_road.front()));
            _road.pop_front();
            auto current = _tr->getPosition();
            _tr->setDirection(Math::GetDirectionFromTo(current, next));
            _tr->setVelocity((next.x - current.x) / 20.0f, (next.y - current.y) / 20.0f);
        }
        else
        {
            _state = BossState::kIdle;
            _tr->setVelocity(0, 0);
        }
    }

private:
    Transform* _tr;
    Animator* _at;
    CoolTimer _move_timer;
    CoolTimer _move_step_timer;
    std::deque<std::pair<int, int>> _road;
    BossState _state;
    int _hp;
};