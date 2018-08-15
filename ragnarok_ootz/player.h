#pragma once

#include <typeindex>
#include <iomanip>
#include <sstream>

#include "game_object.h"
#include "transform.h"
#include "animator.h"
#include "renderer.h"
#include "a_star.h"
#include "lightning_spell.h"
#include "carrot_missile.h"
#include "sound_manager.h"
#include "critical_damage.h"
#include "damage.h"

enum class PlayerState
{
    kIdle = 0,
    kMove = 1,
    kAttack = 11,
    kReceivingDamage = 6,
    kDead = 8
};

class Player : public GameObject
{
public:
    Player()
        : _move_step_timer(CoolTimer(0.2f, std::bind(&Player::moveStep, this)))
        , _presence_timer(CoolTimer(0.5f, std::bind(&Player::showPresence, this)))
        , _collider_timer(CoolTimer(0.1f, std::bind(&Player::setCollider, this)))
        , _road(std::deque<std::pair<int, int>>{})
        , _lightning_cool_time(5.0f)
        , _lightning_cool_down(0.0f)
        , _carrot_missile_cool_time(2.0f)
        , _carrot_missile_cool_down(0.0f)
        , _hp(200000)
        , _sp(100)
        , _is_game_overed(false)
        , _is_clicked(false)
        , _fade_out(false)
        , _fade_alpha(255)
        , _fade_alpha2(120)
    {
        setTag(Tag::kPlayer);
        _state = PlayerState::kIdle;

        _tr = setComponent<Transform>();
        _tr->setPosition(Space::MapToWorld(Vector2(6, 47)));
        _tr->setSize(50, 50);

        GameObject* body = new GameObject;
        auto body_tr = body->setComponent<Transform>();
        body_tr->setPosition(Vector2());
        _body_at = body->setComponent<Animator>();
        _body_at->setActKey(1);
        _body_at->setSpritesKey(1);
        _body_at->setAnimationKey(static_cast<int>(PlayerState::kIdle));
        auto body_rd = body->setComponent<Renderer>();
        registerChild(body);

        GameObject* head = new GameObject;
        auto head_tr = head->setComponent<Transform>();
        head_tr->setPosition(Vector2());
        _head_at = head->setComponent<Animator>();
        _head_at->setActKey(0);
        _head_at->setSpritesKey(0);
        _head_at->setAnimationKey(static_cast<int>(PlayerState::kIdle));
        auto head_rd = head->setComponent<Renderer>();
        registerChild(head);

        GameObject* shadow = new GameObject;
        shadow->setLayer(Layer::kShadow);
        auto shadow_tr = shadow->setComponent<Transform>();
        shadow_tr->setPosition(0, 0);
        auto shadow_rd = shadow->setComponent<Renderer>();
        shadow_rd->setSpritesKey(3);
        shadow_rd->accessRenderInfo()->alpha = 100;
        registerChild(shadow);
    }

    virtual void update() override
    {
        if (_is_clicked && _fade_out)
        {
            _lightning_cool_down -= Time::DeltaTime();
            _carrot_missile_cool_down -= Time::DeltaTime();

            if (_state != PlayerState::kAttack && _state != PlayerState::kReceivingDamage && _state != PlayerState::kDead)
            {
                if (Input::IsOnceKeyDown(VK_LBUTTON))
                {
                    Vector2 mouse_pos = Input::GetMousePosition();

                    setDirection(mouse_pos);

                    _road = AStar{}(SceneManager{}()->getMapOfCurrentScene(),
                                    Space::WorldToMap(_tr->getPosition()).toIntPair(),
                                    Space::WorldToMap(mouse_pos).toIntPair());
                }

                _move_step_timer();
                _tr->move();

                if (Input::IsOnceKeyDown('Q'))
                {
                    auto pos = Space::WorldToMap(Input::GetMousePosition());
                    if (!(pos.x < 0 || pos.y < 0 || pos.x > 57 || pos.y > 50))
                        if (!SceneManager{}()->getMapOfCurrentScene()[static_cast<int>(pos.y)][static_cast<int>(pos.x)])
                            if (_lightning_cool_down <= 0.0f)
                            {
                                _lightning_cool_down = _lightning_cool_time;

                                _state = PlayerState::kAttack;

                                setDirection(pos);

                                fireLightning();
                            }
                }

                if (Input::IsOnceKeyDown('W'))
                {
                    auto pos = Space::WorldToMap(Input::GetMousePosition());
                    if (!(pos.x < 0 || pos.y < 0 || pos.x > 57 || pos.y > 50))
                        if (!SceneManager{}()->getMapOfCurrentScene()[static_cast<int>(pos.y)][static_cast<int>(pos.x)])
                            if (_carrot_missile_cool_down <= 0.0f)
                            {
                                _carrot_missile_cool_down = _carrot_missile_cool_time;

                                _state = PlayerState::kAttack;

                                setDirection(pos);

                                fireCarrotMissile();
                            }
                }
            }

            _presence_timer();
            _collider_timer();

            for (auto c : getChildren())
                if (auto tr = c->getComponent<Transform>())
                    tr->setDirection(_tr->getDirection());

            updateState();
        }
        else if (_is_clicked)
        {
            _fade_alpha -= 6;
            _fade_alpha2 -= 3;

            if (_fade_alpha <= 5 || _fade_alpha2 <= 3)
                _fade_out = true;
        }
        else
        {
            if (Input::IsOnceKeyDown(VK_LBUTTON))
            {
                _is_clicked = true;
            }
        }

        if (GameObjectManager::FindGameObjectsWithTag(Tag::kBoss).empty())
            _jin_boss_spawned = true;

        if (_jin_boss_spawned && _warn_x_factor < 1200)
        {
            _warn_x_factor += 10;
        }
    }

    virtual void render(const HDC& hdc) override {
        for (auto map_pos : _road)
            SpriteManager{}()->getSprite(6)->drawSmart(hdc, Space::MapToWorld(Vector2::From(map_pos)), Vector2::one, 60, 0.0f);

        // fake hp window
        SpriteManager{}()->getSprite(11)->drawSmart(hdc, _tr->getPosition() + Vector2(0, 22), Vector2::one, 160, 0.0f);
        auto image = SpriteManager{}()->getSprite(12);
        auto pos = Vector2(771, 468);
        auto size = Vector2(image->getSize().x * _hp / 200000.0f, image->getSize().y);
        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = 160;
        bf.AlphaFormat = 0;
        AlphaBlend(hdc,
                   static_cast<int>(pos.x),
                   static_cast<int>(pos.y),
                   static_cast<int>(size.x),
                   static_cast<int>(size.y),
                   image->getDC(),
                   0,
                   0,
                   static_cast<int>(size.x),
                   static_cast<int>(size.y),
                   bf);

        //SpriteManager{}()->getSprite(12)->drawSmart(hdc, _tr->getPosition() + Vector2(0, 20), Vector2::one, 160, 0.0f); // hp
        SpriteManager{}()->getSprite(13)->drawSmart(hdc, _tr->getPosition() + Vector2(0, 24), Vector2::one, 160, 0.0f); // sp

        // spell window
        SpriteManager{}()->getSprite(17)->drawSmart(hdc, _tr->getPosition() + Vector2(-688, -402), Vector2::one * 3, 255, 0.0f);
        SpriteManager{}()->getSprite(14)->drawSmart(hdc, _tr->getPosition() + Vector2(-752, -402), Vector2::one * 3, 255, 0.0f);
        SpriteManager{}()->getSprite(15)->drawSmart(hdc, _tr->getPosition() + Vector2(-665,-402), Vector2::one * 3, 255, 0.0f);

        // fake chatting window
        //SpriteManager{}()->getSprite(16)->drawSmart(hdc, _tr->getPosition() + Vector2(0, 350), Vector2::one, 160, 0.0f);

        // mini map
        SpriteManager{}()->getSprite(18)->drawSmart(hdc, _tr->getPosition() + Vector2(550, -250), Vector2::one, 100);
        SpriteManager{}()->getSprite(0)->drawSmart(hdc, _tr->getPosition() + Vector2(550, -250) + (_tr->getPosition() - Vector2(2900, 2550) * 0.5f) * 0.125f, Vector2(0.75f, 0.75f), 192);

        renderSpellCoolDown(hdc, _lightning_cool_time, _lightning_cool_down, Vector2(-752, -402));
        renderSpellCoolDown(hdc, _carrot_missile_cool_time, _carrot_missile_cool_down, Vector2(-665, -402));

        HFONT font = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("Malgun"));
        HFONT oldfont = (HFONT)SelectObject(hdc, font);
        SetTextColor(hdc, Color::kWhite);

        std::string str = "잔여 몬스터 수 : " + std::to_string(GameObjectManager::FindGameObjectsWithTag(Tag::kBoss).size());
        TextOut(hdc, 600, 30, str.c_str(), str.size());

        SelectObject(hdc, oldfont);
        DeleteObject(font);

        if (!_fade_out)
        {
            BYTE alpha = _fade_alpha;
            if (alpha <= 0)
                alpha = 0;
            SpriteManager{}()->getSprite(23)->drawSmart(hdc, _tr->getPosition() + Vector2(-500, 200), Vector2::one * 1.5, alpha, 0);

            alpha = _fade_alpha2;
            if (alpha <= 0)
                alpha = 0;
            SpriteManager{}()->getSprite(24)->drawSmart(hdc, _tr->getPosition() + Vector2(200, 250), Vector2::one * 1.2, alpha, 0);

            HFONT font = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("./malgun.ttf"));
            HFONT oldfont = (HFONT)SelectObject(hdc, font);
            SetTextColor(hdc, RGB(_fade_alpha, _fade_alpha, _fade_alpha));

            //std::string str = "여기인가... (진지함)";
            //TextOut(hdc, 650, 600, str.c_str(), str.size());
            //str = "화면을 클릭하세요.";
			std::string str = "(여기 있는 몬스터들을 물리치면";
			TextOut(hdc, 650, 600, str.c_str(), str.size());
			str = "대마왕이 나온뎄지...)";
			TextOut(hdc, 650, 665, str.c_str(), str.size());
			str = "화면을 클릭하세요.";
            TextOut(hdc, 650, 730, str.c_str(), str.size());
            SelectObject(hdc, oldfont);
            DeleteObject(font);
        }

        if (_jin_boss_spawned && _warn_x_factor < 1200)
        {
            SpriteManager{}()->getSprite(25)->drawSmart(hdc, _tr->getPosition() + Vector2(_warn_x_factor, 250) * -1, Vector2::one * 5, 255, 0);
            SpriteManager{}()->getSprite(25)->drawSmart(hdc, _tr->getPosition() + Vector2(_warn_x_factor, 250), Vector2::one * 5, 255, 0);

            HFONT font = CreateFont(200, 0, 0, 0, FW_HEAVY, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("Malgun"));
            HFONT oldfont = (HFONT)SelectObject(hdc, font);
            SetTextColor(hdc, Color::kYellow);

            std::string str = "진★보스 출현!!";
            TextOut(hdc, 200, 350, str.c_str(), str.size());

            SelectObject(hdc, oldfont);
            DeleteObject(font);
        }

        //str = std::to_string(_warn_x_factor);
        //TextOut(hdc, 200, 300, str.c_str(), str.size());

        //GameObject* cut = new GameObject;
        //auto cut_tr = cut->setComponent<Transform>();
        //cut_tr->setPosition(Vector2(-500, 200));
        //auto cut_rd = cut->setComponent<Renderer>();
        //cut_rd->setSpritesKey(23);
        //cut_rd->accessRenderInfo()->scale = Vector2::one * 1.5;
        //registerChild(cut);

        //GameObject* chat = new GameObject;
        //auto chat_tr = chat->setComponent<Transform>();
        //chat_tr->setPosition(Vector2(200, 250));
        //auto chat_rd = chat->setComponent<Renderer>();
        //chat_rd->setSpritesKey(24);
        //chat_rd->accessRenderInfo()->scale = Vector2::one * 1.2;
        //chat_rd->accessRenderInfo()->alpha = 122;
        //registerChild(chat);

        //std::string hp_str = std::to_string(_hp);
        //TextOut(hdc, 500, 0, hp_str.c_str(), hp_str.size());
    }

    virtual void onCollisionEnter(GameObject* other, RECT const& area) override
    {
        if (_is_game_overed) return;

        switch (other->getTag())
        {
        case Tag::kBossAttack:
        {
            _state = PlayerState::kReceivingDamage;

            DotDamage* damage_go = static_cast<DotDamage*>(other);
            int damage = damage_go->getDamage();
            _hp -= damage;

            if (damage_go->isCritical())
                RegisterGameObject(new CriticalDamageEffect(_tr->getPosition() + Vector2(0, -50), damage * 2));
            else
                RegisterGameObject(new DamageEffect(_tr->getPosition() + Vector2(0, -50), damage));

            Destroy(other->getInstanceId());
            SoundManager{}()->play(RagnarokSound::Player::Hit{});

            if (_hp <= 0)
            {
                _state = PlayerState::kDead;

                GameObject* game_over = new GameObject;
                game_over->setLayer(Layer::kEffect);
                auto game_over_tr = game_over->setComponent<Transform>();
                game_over_tr->setPosition(0, 0);
                _game_over_at = game_over->setComponent<Animator>();
                _game_over_at->setActKey(7);
                _game_over_at->setSpritesKey(22);
                auto game_over_rd = game_over->setComponent<Renderer>();
                game_over_rd->accessRenderInfo()->scale = Vector2::one * 2;
                registerChild(game_over);

                _is_game_overed = true;
            }
        }
        break;
        case Tag::kBossSpellDamage:
        {
            _state = PlayerState::kReceivingDamage;

            DotDamage* damage_go = static_cast<DotDamage*>(other);
            int damage = damage_go->getDamage();
            _hp -= damage;

            if (damage_go->isCritical())
                RegisterGameObject(new CriticalDamageEffect(_tr->getPosition() + Vector2(0, 0), damage * 2));
            else
                RegisterGameObject(new DamageEffect(_tr->getPosition() + Vector2(0, 0), damage));

            Destroy(other->getInstanceId());
            SoundManager{}()->play(RagnarokSound::Player::Hit{});

            if (_hp <= 0)
            {
                _state = PlayerState::kDead;

                GameObject* game_over = new GameObject;
                game_over->setLayer(Layer::kEffect);
                auto game_over_tr = game_over->setComponent<Transform>();
                game_over_tr->setPosition(0, 0);
                _game_over_at = game_over->setComponent<Animator>();
                _game_over_at->setActKey(7);
                _game_over_at->setSpritesKey(22);
                auto game_over_rd = game_over->setComponent<Renderer>();
                game_over_rd->accessRenderInfo()->scale = Vector2::one * 2;
                registerChild(game_over);

                _is_game_overed = true;
            }
        }
        break;
        }
    }

    void moveStep()
    {
        if (!_road.empty())
        {
            _state = PlayerState::kMove;

            Vector2 next_pos = Space::MapToWorld(Vector2::From(_road.front()));
            _road.pop_front();

            setDirection(next_pos);
            _tr->setVelocity((next_pos - _tr->getPosition()) * 0.11f);
        }
        else
        {
            _state = PlayerState::kIdle;

            _tr->setVelocity(Vector2::zero);
        }
    }

    void setDirection(const Vector2& to)
    {
        _tr->setDirection(Math::GetDirectionFromTo(_tr->getPosition(), to));
    }

    void fireLightning()
    {
        auto pos = Space::WorldToMap(Input::GetMousePosition());
        if (!SceneManager{}()->getMapOfCurrentScene().getAttributes(pos))
            RegisterGameObject(new LightningSpell(Tag::kPlayerSpell, Space::MapToWorld(pos)));
    }

    void fireCarrotMissile()
    {
        auto pos = Space::WorldToMap(Input::GetMousePosition());
        if (!SceneManager{}()->getMapOfCurrentScene().getAttributes(pos))
            RegisterGameObject(new CarrotMissile(Tag::kPlayerSpell, Space::MapToWorld(pos)));
    }

    void showPresence()
    {
        RegisterGameObject(new DotDamage(Tag::kPlayerIAm, _tr->getPosition(), Space::MapToWorld(Vector2::one * 6), 0, false));
    }

    void setCollider()
    {
        RegisterGameObject(new DotDamage(Tag::kPlayerCollider, _tr->getPosition(), Space::MapToWorld(Vector2::one), 0, false));
    }

    void updateState()
    {
        switch (_state)
        {
        case PlayerState::kIdle:
        {

        }
        break;
        case PlayerState::kMove:
        {

        }
        break;
        case PlayerState::kAttack:
        {
            if (_body_at->getFrameKey() == 7)
                _state = PlayerState::kIdle;
        }
        break;
        case PlayerState::kReceivingDamage:
        {
            if (_body_at->getFrameKey() == 2)
                _state = PlayerState::kIdle;
        }
        break;
        case PlayerState::kDead:
        {
            if (_game_over_at->getFrameKey() == 50)
                _game_over_at->setFrameKey(49);
        }
        break;
        }

        _body_at->setAnimationKey(static_cast<int>(_state));
        _head_at->setAnimationKey(static_cast<int>(_state));
    }

    void renderSpellCoolDown(const HDC hdc, const float cool_time, const float cool_down, const Vector2& pos_from_center)
    {
        if (cool_down > 0)
        {
            auto sprite = SpriteManager{}()->getSprite(20);
            HDC hdc2 = CreateCompatibleDC(sprite->getDC());
            HBITMAP hbit2 = CreateCompatibleBitmap(sprite->getDC(), static_cast<int>(sprite->getSize().x), static_cast<int>(sprite->getSize().y));
            HBITMAP holdbit2 = (HBITMAP)SelectObject(hdc2, hbit2);
            BitBlt(hdc2, 0, 0, static_cast<int>(sprite->getSize().x), static_cast<int>(sprite->getSize().y), sprite->getDC(), 0, 0, SRCCOPY);

            float elapsed_time = cool_time - cool_down;
            float angle = elapsed_time * 360.0f / cool_time;

            float angle2 = angle + 270.0f;
            if (angle2 >= 360.0f)
                angle2 -= 360.0f;

            float radian = Math::DegreesToRadians(angle2);
            int width = static_cast<int>(sprite->getSize().x);
            int height = static_cast<int>(sprite->getSize().y);
            int half_width = width / 2;
            int half_height = height / 2;

            int number_of_points = 3;
            POINT pt[7];
            pt[0].x = half_width;
            pt[0].y = half_height;
            pt[1].x = half_width;
            pt[1].y = -1;

            if (angle > 45.0f)
            {
                pt[2].x = width + 1;
                pt[2].y = -1;
                number_of_points++;
            }

            if (angle > 135.0f)
            {
                pt[3].x = width;
                pt[3].y = height;
                number_of_points++;
            }

            if (angle > 225.0f)
            {
                pt[4].x = -1;
                pt[4].y = height;
                number_of_points++;
            }

            if (angle > 315.0f)
            {
                pt[5].x = -1;
                pt[5].y = -1;
                number_of_points++;
            }

            pt[number_of_points - 1].x = half_width + static_cast<LONG>(cosf(radian) * width);
            pt[number_of_points - 1].y = half_height + static_cast<LONG>(sinf(radian) * height);

            HPEN pen = CreatePen(PS_SOLID, 1, Color::kMagenta);
            HPEN oldpen = (HPEN)SelectObject(hdc2, pen);
            HBRUSH brush = CreateSolidBrush(Color::kMagenta);
            HBRUSH oldbrush = (HBRUSH)SelectObject(hdc2, brush);

            SetPolyFillMode(hdc2, WINDING);
            Polygon(hdc2, pt, number_of_points);

            Image::DrawStretchTransAlpha(hdc, _tr->getPosition() + GameObjectManager::GetFactorOfPosition() + pos_from_center, hdc2, sprite->getSize(), Vector2::one * 3, Color::kMagenta, 128);

            SelectObject(hdc2, oldpen);
            DeleteObject(pen);
            SelectObject(hdc2, oldbrush);
            DeleteObject(brush);
            SelectObject(hdc2, holdbit2);
            DeleteObject(hbit2);
            DeleteDC(hdc2);
        }

        HFONT font = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("Malgun"));
        HFONT oldfont = (HFONT)SelectObject(hdc, font);
        SetTextColor(hdc, Color::kRed);

        auto pos = _tr->getPosition() + GameObjectManager::GetFactorOfPosition() + pos_from_center + Vector2(-5, -10);
        std::string str;
        if (cool_down >= 1.0f && cool_down <= cool_time)
            str = std::to_string(static_cast<int>(cool_down));
        else if (cool_down > 0.0f && cool_down < 1.0f)
        {
            pos += Vector2(-10, 0);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << cool_down;
            str = ss.str();
        }
        else
            str = "";

        TextOut(hdc, static_cast<int>(pos.x), static_cast<int>(pos.y), str.c_str(), str.size());

        SelectObject(hdc, oldfont);
        DeleteObject(font);
    }

    bool isGameOver()
    {
        return _is_game_overed;
    }

private:
    Transform* _tr;

    CoolTimer _move_step_timer;
    CoolTimer _presence_timer;
    CoolTimer _collider_timer;

    std::deque<std::pair<int, int>> _road;

    const float _lightning_cool_time;
    float _lightning_cool_down;

    const float _carrot_missile_cool_time;
    float _carrot_missile_cool_down;

    PlayerState _state;
    Animator* _body_at;
    Animator* _head_at;
    Animator* _game_over_at;

    int _hp;
    int _sp;

    bool _is_game_overed;
    bool _is_clicked;
    bool _fade_out;
    BYTE _fade_alpha;
    BYTE _fade_alpha2;

    bool _jin_boss_spawned;
    float _warn_x_factor;
};
