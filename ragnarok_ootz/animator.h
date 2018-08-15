#pragma once

#include "component.h"
#include "cool_timer.h"

class Animator : public Component
{
public:
    Animator(GameObject* const& owner);

    virtual void update() override;

    void updateCoolTime();

    bool isLastFrame();

    void setActKey(int const& key);
    int getActKey() const;
    void setAnimationKey(int const& key);
    int getAnimationKey() const;
    void setActionKey(int const& key);
    int getActionKey() const;
    void setFrameKey(int const& key);
    int getFrameKey() const;
    void setSpritesKey(int const& key);
    int getSpritesKey() const;

private:
    int _sprites_key;
    int _act_key;
    int _animation_key;
    int _previous_animation_key;
    int _action_key;
    int _frame_key;

    CoolTimer _cool_timer;
};
