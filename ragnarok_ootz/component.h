#pragma once

class GameObject;

class Component
{
public:
    Component(GameObject* const& owner) 
        : _owner(owner)
    {
    }

    virtual ~Component() 
    {
    }

    virtual void init()
    {
    }

    virtual void update()
    {
    }

    virtual GameObject* getOwner() const final
    {
        return _owner;
    }

private:
    GameObject* _owner;
};
