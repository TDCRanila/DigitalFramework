#include <GameWorld/GameWorld.h>

namespace DFW
{
    void GameWorld::Init()
    {
        _ecs = MakeUnique<DECS::ECSModule>();
        _ecs->Init();

        _world_root = SpawnGameObject(SpawnInfo());
        _world_root.AddComponent<WorldRootTagComponent>();
    }

    void GameWorld::Terminate()
    {
        _ecs->Terminate();
        _ecs.reset();
    }

    void GameWorld::Update()
    {
        _ecs->UpdateECS();
    }

    void GameWorld::RenderImGui()
    {
        _ecs->UpdateECSImGui();
    }

    GameObject GameWorld::SpawnGameObject(SpawnInfo const& a_spawn_info)
    {
        return SpawnGameObject<"Entity">(a_spawn_info);
    }

    void GameWorld::DestroyGameObject(GameObject const& a_game_object)
    {
        if (!IsEntityAGameObject(a_game_object))
        {
            DFW_WARNLOG("Attempting to destroy a GameObject which it is not. ");
            return;
        }

        _ecs->EventHandler().Broadcast<GameObjectDestroyedEvent>(a_game_object);

        _ecs->GetRegistry().DestroyEntity(a_game_object);
    }

    GameObject GameWorld::GetGameObject(DUID const a_game_object_id) const
    {
        GameObject found_entity(_ecs->GetRegistry().GetEntity(a_game_object_id));
        
        if (found_entity.IsEntityValid() && IsEntityAGameObject(found_entity))
            return found_entity;
        else
            return GameObject();
    }

    GameObject GameWorld::GetGameObject(std::string const& a_game_object_name) const
    {
        GameObject found_entity(_ecs->GetRegistry().GetEntity(a_game_object_name));

        if (found_entity.IsEntityValid() && IsEntityAGameObject(found_entity))
            return found_entity;
        else
            return GameObject();
    }

    bool GameWorld::IsEntityAGameObject(Entity const& a_entity) const
    {
        return a_entity.TryGetComponent<GameObjectTagComponent>();
    }

} // End of namespace ~ DFW. 
