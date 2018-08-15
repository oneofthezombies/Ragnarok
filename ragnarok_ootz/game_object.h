#pragma once

#include <unordered_map>
#include <type_traits>
#include <Windows.h>
#include <typeinfo>
#include <cassert>
#include <deque>

#include "tag.h"
#include "layer.h"

class Component;

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    virtual void init();
    virtual void release();
    virtual void update();
    virtual void render(HDC const& hdc);
    virtual void onCollisionEnter(GameObject* other, RECT const& area);

    virtual void initRecursive() final;
    virtual void releaseRecursive() final;
    virtual void updateRecursive() final;
    virtual void renderRecursive(HDC const& hdc) final;

    virtual void setParent(GameObject* parent) final;
    virtual GameObject* getParent() const;
    virtual void registerChild(GameObject* child) final;
    virtual std::deque<GameObject*> getChildren() const final;

    virtual void setInstanceId(int const& instanceId) final;
    virtual int getInstanceId() const final;
    virtual void setTag(Tag const& _tag) final;
    virtual Tag getTag() const final;
    virtual void setLayer(Layer const& _layer) final;
    virtual Layer getLayer() const final;

    template<typename ComponentType>
    ComponentType* setComponent();
    template<typename ComponentType>
    ComponentType* getComponent();

    static void RegisterGameObject(GameObject* go);
    static void Destroy(GameObject* go);
    static void Destroy(const int instance_id);

private:
    int _instance_id;
    Tag _tag;
    Layer _layer;
    std::unordered_map<size_t, Component*> _components;

    GameObject* _parent;
    std::deque<GameObject*> _children;
};

template<typename T>
T* GameObject::setComponent()
{
    assert((std::is_base_of<Component, T>::value) && "GameObject::setComponent()");

    size_t key = typeid(T).hash_code();
    T* component = nullptr;
    if (_components.find(key) == _components.end())
    {
        component = new T(this);
        _components.insert(std::make_pair(key, component));
    }
    else
        component = static_cast<T*>(_components[key]);

    return component;
}

template<typename T>
T* GameObject::getComponent()
{
    auto result = _components.find(typeid(T).hash_code());
    if (result != _components.end())
        return static_cast<T*>(result->second);

    return nullptr;
}
