#pragma once

#include "vector2.h"

#include "component.h"
#include "direction.h"

class Transform : public Component
{
public:
    Transform(GameObject* const& owner)
        : Component(owner)
        , _size(Vector2 {})
        , _position(Vector2 {})
        , _velocity(Vector2 {})
        , _direction(Direction::kDown)
    {
    }

    virtual ~Transform()
    {
    }

    void setSize(Vector2 const& size)
    {
        _size.x = size.x;
        _size.y = size.y;
    }

    void setSize(float const& x, float const& y)
    {
        _size.x = x;
        _size.y = y;
    }

    Vector2 getSize() const
    {
        return _size;
    }

    template<typename T1, typename T2>
    void setPosition(T1&& x, T2&& y)
    {
        _position.x = static_cast<float>(std::forward<T1>(x));
        _position.y = static_cast<float>(std::forward<T2>(y));
    }

    void setPosition(Vector2 const& position)
    {
        _position.x = position.x;
        _position.y = position.y;
    }

    void setPosition(float const& x, float const& y)
    {
        _position.x = x;
        _position.y = y;
    }

    Vector2 getPosition() const
    {
        return _position;
    }

    void setVelocity(Vector2 const& velocity)
    {
        _velocity.x = velocity.x;
        _velocity.y = velocity.y;
    }

    void setVelocity(float const& x, float const& y)
    {
        _velocity.x = x;
        _velocity.y = y;
    }

    Vector2 getVelocity() const
    {
        return _velocity;
    }

    void move()
    {
        _position.x += _velocity.x;
        _position.y += _velocity.y;
    }

    void move(float const& velocityX, float const& velocityY)
    {
        moveX(velocityX);
        moveY(velocityY);
    }

    void move(Vector2 const& velocity)
    {
        moveX(velocity.x);
        moveY(velocity.y);
    }

    void moveX(float const& velocityX)
    {
        _position.x += velocityX;
    }

    void moveY(float const& velocityY)
    {
        _position.y += velocityY;
    }

    void setDirection(Direction const& direction)
    {
        _direction = direction;
    }

    Direction getDirection() const
    {
        return _direction;
    }

    Vector2 getPositionOfWorldSpace()
    {
        auto pos = _position;
        auto p = getOwner()->getParent();
        while (p)
        {
            if (auto p_tr = p->getComponent<Transform>())
                pos += p_tr->getPosition();

            p = p->getParent();
        }

        return pos;
    }

    RECT toRect()
    {
        float half_width = _size.x / 2.0f;
        float half_height = _size.y / 2.0f;
        return RECT
        {
            static_cast<LONG>(_position.x - half_width), static_cast<LONG>(_position.y - half_height),
            static_cast<LONG>(_position.x + half_width), static_cast<LONG>(_position.y + half_height)
        };
    }

private:
    Vector2 _size;
    Vector2 _position;
    Vector2 _velocity;
    Direction _direction;
};
