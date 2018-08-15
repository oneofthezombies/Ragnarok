#include "animator.h"

#include "game_object.h"
#include "transform.h"

#include "act_manager.h"
#include "sprite_manager.h"

Animator::Animator(GameObject* const& owner)
    : Component(owner)
    , _sprites_key(0)
    , _act_key(0)
    , _animation_key(0)
    , _previous_animation_key(0)
    , _action_key(0)
    , _frame_key(0)
    , _cool_timer(0.1f)
{
}

void Animator::update()
{
    if (auto tr = getOwner()->getComponent<Transform>())
        _action_key = static_cast<int>(tr->getDirection());

    if (_cool_timer())
    {
        _frame_key++;
        _frame_key %= ActManager {}()->getAnimation(_act_key, _animation_key)->number_of_frames;
    }

    updateCoolTime();
}

void Animator::updateCoolTime()
{
    if (_previous_animation_key != _animation_key)
    {
        _cool_timer.setCoolTime(ActManager {}()->getAct(_act_key)->animations[_animation_key]->interval_time);
        _previous_animation_key = _animation_key;
        _frame_key = 0;
    }
}

bool Animator::isLastFrame()
{
    return _frame_key == ActManager{}()->getFrame(_act_key, _animation_key, _action_key, _frame_key)->size() - 1;
}

void Animator::setActKey(int const& key)
{
    _act_key = key;
}

int Animator::getActKey() const
{
    return _act_key;
}

void Animator::setAnimationKey(int const& key)
{
    _animation_key = key;
}

int Animator::getAnimationKey() const
{
    return _animation_key;
}

void Animator::setActionKey(int const& key)
{
    _action_key = key;
}

int Animator::getActionKey() const
{
    return _action_key;
}

void Animator::setFrameKey(int const& key)
{
    _frame_key = key;
}

int Animator::getFrameKey() const
{
    return _frame_key;
}

void Animator::setSpritesKey(int const& key)
{
    _sprites_key = key;
}

int  Animator::getSpritesKey() const
{
    return _sprites_key;
}
