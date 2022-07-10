#include <GameWorld/GameSystem.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Objects/ECSEntity.h>

#include <CoreSystems/CoreServices.h>

namespace DFW
{
    void GameSystem::Init()
    {
        entity_manager = &CoreService::GetECS()->EntityManager();
        DFW_ASSERT(entity_manager);
    }

    void GameSystem::Terminate()
    {
    }

    void GameSystem::DestroyGameObject(GameObject const& a_game_object)
    {
        ECSEventHandler().Broadcast<GameObjectDestroyedEvent>(a_game_object);

        entity_manager->DestroyEntity(a_game_object);
    }


    GameObject GameSystem::GetGameObject(DUID const a_game_object_id, DECS::Universe& a_universe) const
    {
        Entity entity(entity_manager->GetEntity(a_game_object_id, a_universe));
        if (IsGameObject(entity))
            return GameObject(entity);
        else
            return GameObject();
    }

    bool GameSystem::IsGameObject(Entity const& a_entity) const
    {
        return entity_manager->TryGetComponent<GameObjectTagComponent>(a_entity) ? true : false;
    }

} // End of namespace ~ DFW. 
