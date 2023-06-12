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
    struct WorldRootTagComponent : DECS::Component::StrictRegistrar<WorldRootTagComponent> { };

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
        inline DECS::ECSModule& GetECS() { return *_ecs; }

        template <StringLiteral game_object_type_name>
        GameObject SpawnGameObject(SpawnInfo const& a_spawn_info);
        GameObject SpawnGameObject(SpawnInfo const& a_spawn_info);

        void DestroyGameObject(Entity const& a_game_object);

        GameObject GetGameObject(DUID const a_game_object_id) const;
        GameObject GetGameObject(std::string const& a_game_object_name) const;

        bool IsEntityAGameObject(GameObject const& a_game_object) const;

    private:
        GameObject _world_root;
        UniquePtr<DECS::ECSModule> _ecs;

    };

#pragma region Template Function Implementation

    template <StringLiteral game_object_type_name>
    GameObject GameWorld::SpawnGameObject(SpawnInfo const& a_spawn_info)
    {
        GameObject game_object(_ecs->EntityManager().CreateEntity<game_object_type_name>(_ecs->GetRegistry()));

        // Setup additional Entity data.
        game_object.SetName(a_spawn_info.name);
        game_object.AddComponent<TransformComponent>(a_spawn_info.transform);
        game_object.SetParent(a_spawn_info.parent.IsEntityValid() ? a_spawn_info.parent : _world_root);

        // GameObject Related setup.
        game_object.AddComponent<GameObjectTagComponent>();

        _ecs->EventHandler().Broadcast<GameObjectCreatedEvent>(game_object);

        return game_object;
    }

#pragma endregion

} // End of namespace ~ DFW.
