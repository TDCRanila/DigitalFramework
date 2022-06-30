#pragma once

#include <GameWorld/GameObject.h>
#include <GameWorld/GameEvents.h>
#include <GameWorld/GameObjectTagComponent.h>
#include <GameWorld/SpawnInfo.h>
#include <GameWorld/TransformComponent.h>

#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/ECS/Managers/ECSEntityManager.h>

#include <CoreSystems/Events/EventDispatcher.h>

namespace DFW
{
    // FW Declare.
    struct TransformComponent;

    namespace DECS
    {
        class Universe;
    } // End of namespace ~ DECS.

    template <typename GameObjectType>
    concept IsValidGameObjectType = IsBasedOf<GameObjectType, GameObject>;

    class GameSystem final : public DECS::System::Registrar<GameSystem>
    {
    public:
        GameSystem() = default;
        virtual ~GameSystem() = default;

        template <typename GameObjectType = GameObject, typename... TArgs>
        requires IsValidGameObjectType<GameObjectType>
        GameObject SpawnGameObject(DECS::Universe& a_universe, SpawnInfo const& a_spawn_info, TArgs&&... a_args);

        void DestroyGameObject(GameObject const& a_entity);
        
        GameObject GetGameObject(DUID const a_game_object_id, DECS::Universe& a_universe) const;

        bool IsGameObject(Entity const& a_entity) const;

    private:
        virtual void Init() override;
        virtual void Terminate() override;
    
        DECS::EntityManager* entity_manager;

    };

#pragma region Template Function Implementation

    template <typename GameObjectType, typename... TArgs>
    requires IsValidGameObjectType<GameObjectType>
    GameObject GameSystem::SpawnGameObject(DECS::Universe& a_universe, SpawnInfo const& a_spawn_info, TArgs&&... a_args)
    {
        GameObject game_object(entity_manager->CreateEntity<GameObjectType>(a_universe, std::forward<TArgs>(a_args)...));
        entity_manager->SetEntityName(game_object, a_spawn_info.name);
        entity_manager->AddComponent<GameObjectTagComponent>(game_object);
        entity_manager->AddComponent<TransformComponent>(game_object, a_spawn_info.transform);

        game_object.OnCreate();
                
        ECSEventHandler().Broadcast<GameObjectCreatedEvent>(game_object);

        return game_object;
    }

#pragma endregion

} // End of namespace ~ DFW.
