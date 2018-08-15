#include "game_object.h"

#include "component.h"
#include "renderer.h"

#include "game_object_manager.h"

GameObject::GameObject()
    : _instance_id(-1)
    , _tag(Tag::kIdle)
    , _layer(Layer::kObject)
    , _components(std::unordered_map<size_t, Component*>{})
    , _parent(nullptr)
    , _children(std::deque<GameObject*>{})
{
}

GameObject::~GameObject()
{
    for (auto c : _children)
        delete c;

    for (auto pair : _components)
        delete pair.second;
}

void GameObject::init()
{
}

void GameObject::release()
{
}

void GameObject::update()
{
}

void GameObject::render(HDC const& hdc)
{
}

void GameObject::onCollisionEnter(GameObject* other, RECT const& area)
{
}

void GameObject::initRecursive()
{
    init();

    for (auto p : _components)
        p.second->init();

    for (auto c : _children)
        c->initRecursive();
}

void GameObject::releaseRecursive()
{
    for (auto c : _children)
        c->releaseRecursive();

    release();
}

void GameObject::updateRecursive()
{
    update();

    for (auto p : _components)
        p.second->update();

    for (auto c : _children)
        c->updateRecursive();
}

void GameObject::renderRecursive(HDC const& hdc)
{
    if (auto rd = getComponent<Renderer>())
        rd->render(hdc);

    render(hdc);

    for (auto c : _children)
        c->renderRecursive(hdc);
}

void GameObject::setParent(GameObject* parent)
{
    _parent = parent;
}

GameObject* GameObject::getParent() const
{
    return _parent;
}

void GameObject::registerChild(GameObject* child)
{
    child->setParent(this);
    _children.emplace_back(child);
}

std::deque<GameObject*> GameObject::getChildren() const
{
    return _children;
}

void GameObject::setInstanceId(int const& instance_id)
{
    _instance_id = instance_id;
}

int GameObject::getInstanceId() const
{
    return _instance_id;
}

void GameObject::setTag(Tag const& tag)
{
    _tag = tag;
}

Tag GameObject::getTag() const
{
    return _tag;
}

void GameObject::setLayer(Layer const& layer)
{
    _layer = layer;
}

Layer GameObject::getLayer() const
{
    return _layer;
}

void GameObject::RegisterGameObject(GameObject* go)
{
    GameObjectManager::RegisterGameObject(go);
}

void GameObject::Destroy(GameObject* go)
{
    GameObjectManager::Destroy(go);
}

void GameObject::Destroy(const int instance_id)
{
    GameObjectManager::Destroy(instance_id);
}
