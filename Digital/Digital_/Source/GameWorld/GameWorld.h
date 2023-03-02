#pragma once

#include <GameWorld/GameObject.h>
#include <GameWorld/SpawnInfo.h>
#include <GameWorld/GameEvents.h>
#include <GameWorld/GameObjectTagComponent.h>
#include <GameWorld/TransformComponent.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/EntityManager.h>

#include <CoreSystems/Events/EventDispatcher.h>

namespace DFW
{
    template <typename GameObjectType>
    concept IsValidGameObjectType = IsBasedOf<GameObjectType, GameObject>;

    class GameWorld
    {
    public:
        GameWorld() = default;
        ~GameWorld() = default;

        void Init();
        void Terminate();
        void Update();
        void RenderImGui();

    public:
        // TODO in .cpp or inline
        DECS::ECSModule& GetECS() { return *_ecs; }

        template <typename GameObjectType = GameObject, typename... TArgs> 
        requires IsValidGameObjectType<GameObjectType>
        GameObject SpawnGameObject(SpawnInfo const& a_spawn_info, TArgs&&... a_args);

        void DestroyGameObject(GameObject const& a_entity);
        GameObject GetGameObject(DUID const a_game_object_id) const;
        bool IsGameObject(Entity const& a_entity) const;

    private:
        UniquePtr<DECS::ECSModule> _ecs;

    };

#pragma region Template Function Implementation

    template <typename GameObjectType, typename... TArgs>
        requires IsValidGameObjectType<GameObjectType>
    GameObject GameWorld::SpawnGameObject(SpawnInfo const& a_spawn_info, TArgs&&... a_args)
    {
        DECS::EntityManager& entity_manager = _ecs->EntityManager();

        GameObject game_object(entity_manager.CreateEntity<GameObjectType>(_ecs->GetRegistry(), std::forward<TArgs>(a_args)...));

        game_object.SetName(a_spawn_info.name);
        game_object.AddComponent<GameObjectTagComponent>();
        game_object.AddComponent<TransformComponent>(a_spawn_info.transform);

        game_object.OnCreate();

        _ecs->EventHandler().Broadcast<GameObjectCreatedEvent>(game_object);

        return game_object;
    }

#pragma endregion

} // End of namespace ~ DFW.
