#include <Zynthurism/Game/TestStage.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Input/InputKeys.h>

#include <Modules/ECS/Managers/ECSEntityManager.h>
#include <Modules/ECS/Managers/ECSystemManager.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <Modules/Resource/Mesh/MeshLoader.h>

#include <GameWorld/TransformComponent.h>
#include <GameWorld/Camera/CameraComponent.h>
#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Graphics/ModelComponent.h>
#include <GameWorld/Graphics/SpriteComponent.h>

#include <Utility/FileSystemUtility.h>

namespace DGame
{
    TestStage::TestStage(std::string a_stage_name, bool a_start_disabled)
        : DGame::GameStageBase(a_stage_name, a_start_disabled)
    {
        ecs = DFW::CoreService::GetECS();
    }

    void TestStage::Update()
    {
        // Move the main entity.
        glm::vec3 dir(0.0f);
        if (DFW::CoreService().GetInputSystem()->IsKeyDown(DFW::DInput::DKey::A))
            dir.x -= 1.0f;
        if (DFW::CoreService().GetInputSystem()->IsKeyDown(DFW::DInput::DKey::D))
            dir.x += 1.0f;

        if (DFW::CoreService().GetInputSystem()->IsKeyDown(DFW::DInput::DKey::W))
            dir.y += 1.0f;
        if (DFW::CoreService().GetInputSystem()->IsKeyDown(DFW::DInput::DKey::S))
            dir.y -= 1.0f;

        if (DFW::CoreService().GetInputSystem()->IsKeyDown(DFW::DInput::DKey::Q))
            dir.z += 1.0f;
        if (DFW::CoreService().GetInputSystem()->IsKeyDown(DFW::DInput::DKey::E))
            dir.z -= 1.0f;

        DFW::TimeUnit const dt = DFW::CoreService().GetGameClock()->GetLastFrameDeltaTime();
        float32 const speed = 10.0f;
        DFW::TransformComponent& transform = ecs->EntityManager().GetComponent<DFW::TransformComponent>(main_entity);
        transform.translation.x += dir.x * dt * speed;
        transform.translation.y += dir.y * dt * speed;
        transform.translation.z += dir.z * dt * speed;
    }

    void TestStage::OnAttached()
    {
        // Create Universe.
        _universe = ecs->RegisterUniverse("Main");
        
        // Create Main Camera.
        camera_entity = ecs->EntityManager().CreateEntity(*_universe);
        ecs->EntityManager().AddComponent<DFW::TransformComponent>(camera_entity);

        DFW::CameraSystem* camera_system = ecs->SystemManager().GetSystem<DFW::CameraSystem>();
        DFW::CameraComponent& camera = camera_system->CreateCamera(camera_entity, "camera-one");
        camera_system->ChangeCameraProjPerspective(camera, 90.f, (16.f/9.f), DFW::ClipSpace(0.1f, 5000.f));
        camera_system->SetActiveCamera(camera_entity);

        std::string work_dir;
        DFW::DUtility::GetWorkDirectory(work_dir);
        std::string model_dir(work_dir + DIR_SLASH + "models");

        // XYZ Representation.
        DFW::Entity xyz = ecs->EntityManager().CreateEntity(*_universe);
        DFW::TransformComponent& xyz_transform = ecs->EntityManager().AddComponent<DFW::TransformComponent>(xyz);
        DFW::ModelComponent& xyz_model = ecs->EntityManager().AddComponent<DFW::ModelComponent>(xyz);
        xyz_transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
        xyz_model.model = DFW::DResource::LoadMesh(model_dir + DIR_SLASH + "xyz" + DIR_SLASH + "xyz_10x10m.glb");

        // Sponza Scene.
        DFW::Entity sponza = ecs->EntityManager().CreateEntity(*_universe);
        ecs->EntityManager().AddComponent<DFW::TransformComponent>(sponza);
        DFW::ModelComponent& sponza_model = ecs->EntityManager().AddComponent<DFW::ModelComponent>(sponza);
        sponza_model.model = DFW::DResource::LoadMesh(model_dir + DIR_SLASH + "sponza" + DIR_SLASH + "sponza.gltf");

        // Movable Entity.
        main_entity = ecs->EntityManager().CreateEntity(*_universe);
        DFW::TransformComponent& transform = ecs->EntityManager().AddComponent<DFW::TransformComponent>(main_entity);
        transform.translation = glm::vec3(0.0f, 5.0f, 0.0f);
        transform.scale = glm::vec3(2.0f);
        DFW::ModelComponent& model = ecs->EntityManager().AddComponent<DFW::ModelComponent>(main_entity);
        model.model = DFW::Debug_CreateBasicCube();
    }

    void TestStage::OnRemoved()
    {
    }

} // End of namespace ~ DGame.
