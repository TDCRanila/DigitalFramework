#include <Zynthurism/Game/Level.h>

#include <GameWorld/TransformComponent.h>
#include <GameWorld/Graphics/ModelComponent.h>
#include <GameWorld/Physics/PhysicsComponent.h>

#include <GameWorld/Physics/PhysicsSystem.h>

#include <Modules/Resource/ResourceManager.h>
#include <Modules/Resource/Map/MapData.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/SystemManager.h>

#include <CoreSystems/CoreServices.h>

namespace DGame
{
    DFW::Entity SpawnLevel(DFW::FilePath const& a_map_file, DFW::SpawnInfo const& a_spawn_info, DFW::DECS::ECSModule& a_ecs)
    {
        DFW::ResourceHandle<DFW::DResource::MapData> map = DFW::CoreService::GetResourceManager()->Load<DFW::DResource::MapData>(a_map_file);
        if (!map)
        {
            DFW_ERRORLOG("Failed to spawn a level. Could not load the map file.");
            return DFW::Entity();
        }

        // Setup Level Entity and its childeren.
        DFW::Entity level;
        if (a_spawn_info.parent)
            level = a_ecs.Registry().CreateEntity(a_spawn_info.parent);
        else
            level = a_ecs.Registry().CreateEntity();

        level.SetType<"Level">();
        level.SetName(a_spawn_info.name);

        level.AddComponent<DFW::TransformComponent>(a_spawn_info.transform);
        level.AddComponent<DFW::ModelComponent>(DFW::MakeShared<DFW::DRender::MeshData>(map->mesh_data), nullptr);

        // Create Colliders.
        DFW::PhysicsSystem* const physics_system = a_ecs.SystemManager().GetSystem<DFW::PhysicsSystem>();

        for (DFW::DResource::MapData::ColliderData const& collider_data : map->collider_data)
        {
            DFW::Entity collider_entity = a_ecs.Registry().CreateEntity(level);
            collider_entity.SetType<"LevelPhysicsCollider">();

            DFW::Transform collider_transform = a_spawn_info.transform;
            collider_transform.Translate(collider_data.center_position);
            collider_entity.AddComponent<DFW::TransformComponent>(collider_transform);

            DFW::RigidBodyComponent& rigid_body_component = collider_entity.AddComponent<DFW::RigidBodyComponent>();
            rigid_body_component.body_id = physics_system->CreateMeshRigidBody(collider_transform, *collider_data.shape_settings, JPH::EMotionType::Static);
        }

        return level;
    }

} // End of namespace ~ DGame.
