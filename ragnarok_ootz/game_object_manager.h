#pragma once

#include <unordered_map>
#include <Windows.h>
#include <vector>
#include <deque>

#include "tag.h"
#include "layer.h"

class GameObject;
struct Vector2;

class GameObjectManager
{
public:
    static void Init();
    static void Update();
    static void Render(HDC const& hdc);
    static void Reset();

    static void RegisterGameObject(GameObject* game_object);
    static void Destroy(int const& id);
    static void Destroy(GameObject* game_object);
    static GameObject* FindWithTag(Tag const& tag);
    static std::deque<GameObject*> FindGameObjectsWithTag(Tag const& tag);

    static void NotifyCollisions();
    static void RegisterCollisionRelation(Tag const& actor, Tag const& collosion);

    static Vector2 GetFactorOfPosition();

    static bool IsGameOver();

private:
    static int _game_object_id;
    static std::unordered_map<Tag, std::deque<GameObject*>> _game_objects;
    static std::unordered_map<Tag, std::vector<Tag>> _collision_groups;
    static std::vector<Layer> _layer_order;

    static bool _jin_boss_gen;
};
