#include "game_object_manager.h"

#include "game_object.h"
#include "transform.h"
#include "renderer.h"

#include "cursor.h"
#include "player.h"
#include "boss.h"
#include "jin_boss.h"

int GameObjectManager::_game_object_id = 0;
std::unordered_map<Tag, std::deque<GameObject*>> GameObjectManager::_game_objects;
std::unordered_map<Tag, std::vector<Tag>> GameObjectManager::_collision_groups;
std::vector<Layer> GameObjectManager::_layer_order;
bool GameObjectManager::_jin_boss_gen = false;

void GameObjectManager::Init()
{
    _layer_order.emplace_back(Layer::kShadow);
    _layer_order.emplace_back(Layer::kObject);
    _layer_order.emplace_back(Layer::kEffect);

    RegisterCollisionRelation(Tag::kPlayerLightningDamage, Tag::kBoss);
    RegisterCollisionRelation(Tag::kPlayerCarrotMissileDamage, Tag::kBoss);

    RegisterCollisionRelation(Tag::kPlayerIAm, Tag::kBoss);
    RegisterCollisionRelation(Tag::kPlayerCollider, Tag::kBoss);

    RegisterCollisionRelation(Tag::kBossAttack, Tag::kPlayer);

    RegisterCollisionRelation(Tag::kBossSpellDamage, Tag::kPlayer);

    RegisterGameObject(new Cursor);
    RegisterGameObject(new Player);

    for (int i = 0; i < 5; ++i)
    {
        Boss* boss = new Boss;
        auto tr = boss->getComponent<Transform>();
        std::pair<int, int> pos;
        auto map = SceneManager {}()->getMapOfCurrentScene();
        do
        {
            pos = std::make_pair(Random {}()->getInt(36), Random {}()->getInt(41));
        }
        while (map[pos.second][pos.first]);
        tr->setPosition(Converter::MapSpaceToWorldSpace(pos, map.cell_size));
        RegisterGameObject(boss);
    }

    for (auto group : _game_objects)
        for (auto& go : group.second)
            go->initRecursive();
}

void GameObjectManager::Update()
{
    for (auto group : _game_objects)
        for (auto& go : group.second)
            go->updateRecursive();

    if (_game_objects[Tag::kBoss].empty() && !_jin_boss_gen)
    {
        _jin_boss_gen = true;
        RegisterGameObject(new JinBoss);
        SoundManager{}()->play(RagnarokSound::JinBoss::Spawn{});
    }

    NotifyCollisions();
}

void GameObjectManager::Render(HDC const& hdc)
{
    std::deque<GameObject*> go_queue;
    for (auto group : _game_objects)
        for (auto go : group.second)
            go_queue.emplace_back(go);

    std::unordered_map<Layer, std::map<float, std::vector<GameObject*>>> sorted_game_objects;
    while (!go_queue.empty())
    {
        auto front = go_queue.front();
        Transform* tr = front->getComponent<Transform>();
        float y_order = 0.0f;
        if (tr)
            y_order = tr->getPositionOfWorldSpace().y;
        sorted_game_objects[front->getLayer()][y_order].emplace_back(front);

        for (auto c : front->getChildren())
            go_queue.emplace_back(c);

        go_queue.pop_front();
    }

    for (auto layer : _layer_order)
        for (auto gos : sorted_game_objects[layer])
            for (auto go : gos.second)
            {
                if (auto rd = go->getComponent<Renderer>())
                    rd->render(hdc);
                go->render(hdc);
            }
}

void GameObjectManager::Reset()
{
    for (auto& group : _game_objects)
        for (auto& go : group.second)
            go->releaseRecursive();

    _game_objects.clear();
    _game_object_id = 0;
}

void GameObjectManager::RegisterGameObject(GameObject* game_object)
{
    game_object->setInstanceId(_game_object_id);
    _game_object_id++;

    _game_objects[game_object->getTag()].emplace_back(game_object);
}

void GameObjectManager::Destroy(int const& id)
{
    for (auto& group : _game_objects)
        for (auto it = group.second.begin(); it != group.second.end();)
            if ((*it)->getInstanceId() == id)
                it = group.second.erase(it);
            else
                ++it;
}

void GameObjectManager::Destroy(GameObject* game_object)
{
    for (auto& group : _game_objects)
        for (auto it = group.second.begin(); it != group.second.end();)
            if ((*it) == game_object)
                it = group.second.erase(it);
            else
                ++it;
}

GameObject* GameObjectManager::FindWithTag(Tag const& tag)
{
    return _game_objects[tag].front();
}

std::deque<GameObject*> GameObjectManager::FindGameObjectsWithTag(Tag const& tag)
{
    return _game_objects[tag];
}

void GameObjectManager::NotifyCollisions()
{
    auto collision_groups = _collision_groups;
    for (auto tag_group_i = collision_groups.begin(); tag_group_i != collision_groups.end(); ++tag_group_i)
    {
        auto actors = _game_objects[tag_group_i->first];
        for (auto actor_i = actors.begin(); actor_i != actors.end(); ++actor_i)
        {
            for (auto collider_tag_i = tag_group_i->second.begin(); collider_tag_i != tag_group_i->second.end(); ++collider_tag_i)
            {
                auto colliders = _game_objects[*collider_tag_i];
                for (auto collider_i = colliders.begin(); collider_i != colliders.end(); ++collider_i)
                {
                    RECT area;
                    auto actor_tr = (*actor_i)->getComponent<Transform>();
                    auto collider_tr = (*collider_i)->getComponent<Transform>();
                    if (actor_tr && collider_tr)
                    {
                        if (IntersectRect(&area, &actor_tr->toRect(), &collider_tr->toRect()))
                        {
                            (*collider_i)->onCollisionEnter(*actor_i, area);
                        }
                    }
                }
            }
        }
    }
}

void GameObjectManager::RegisterCollisionRelation(Tag const& actor, Tag const& collosion)
{
    _collision_groups[actor].emplace_back(collosion);
}

Vector2 GameObjectManager::GetFactorOfPosition()
{
    auto player = GameObjectManager::FindWithTag(Tag::kPlayer);
    auto player_tr = player->getComponent<Transform>();
    auto player_pos = player_tr->getPositionOfWorldSpace();
    return Vector2(WindowInfo::kWidth / 2, WindowInfo::kHeight / 2) - player_pos;
}

bool GameObjectManager::IsGameOver()
{
    return static_cast<Player*>(FindWithTag(Tag::kPlayer))->isGameOver();
}
