#include <GameWorld/GameWorld.h>

namespace DFW
{
    void GameWorld::Init()
    {
        _ecs = MakeUnique<DECS::ECSModule>();
        _ecs->Init();
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

    void GameWorld::DestroyGameObject(GameObject const& a_game_object)
    {
        _ecs->EventHandler().Broadcast<GameObjectDestroyedEvent>(a_game_object);

        _ecs->EntityManager().DestroyEntity(a_game_object);
    }


    GameObject GameWorld::GetGameObject(DUID const a_game_object_id) const
    {
        Entity entity(_ecs->EntityManager().GetEntity(a_game_object_id, _ecs->GetRegistry()));
        if (IsGameObject(entity))
            return GameObject(entity);
        else
            return GameObject();
    }

    bool GameWorld::IsGameObject(Entity const& a_entity) const
    {
        return a_entity.TryGetComponent<GameObjectTagComponent>() ? true : false;
    }

} // End of namespace ~ DFW. 
