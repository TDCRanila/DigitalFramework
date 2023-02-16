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
        , _universe(nullptr)
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
        DFW::Transform& transform = ecs->EntityManager().GetComponent<DFW::TransformComponent>(main_entity);
        transform.translation.x += dir.x * dt * speed;
        transform.translation.y += dir.y * dt * speed;
        transform.translation.z += dir.z * dt * speed;
    }

    void TestStage::OnAttached()
    {
        // Create Universe.
        _universe = ecs->RegisterUniverse("Main");
        
        DFW::DECS::EntityManager& em = ecs->EntityManager();

        // Create Main Camera.
        camera_entity = em.CreateEntity(*_universe);
        em.AddComponent<DFW::TransformComponent>(camera_entity);

        DFW::CameraSystem* camera_system = ecs->SystemManager().GetSystem<DFW::CameraSystem>();
        DFW::CameraComponent& camera = camera_system->CreateCamera(camera_entity, "camera-one");
        camera_system->ChangeCameraProjPerspective(camera, 90.f, (16.f/9.f), DFW::ClipSpace(0.1f, 5000.f));
        camera_system->SetActiveCamera(camera_entity);

        std::string work_dir;
        DFW::DUtility::GetWorkDirectory(work_dir);
        std::string model_dir(work_dir + DIR_SLASH + "models");

        // XYZ Representation.
        DFW::Entity xyz = em.CreateEntity(*_universe);
        DFW::Transform& xyz_transform = em.AddComponent<DFW::TransformComponent>(xyz);
        xyz_transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
        DFW::ModelComponent& xyz_model = em.AddComponent<DFW::ModelComponent>(xyz);
        xyz_model.mesh = DFW::DResource::LoadMesh(model_dir + DIR_SLASH + "xyz" + DIR_SLASH + "xyz_10x10m.glb");

        // Sponza Scene.
        DFW::Entity sponza = em.CreateEntity(*_universe);
        em.AddComponent<DFW::TransformComponent>(sponza);
        DFW::ModelComponent& sponza_model = em.AddComponent<DFW::ModelComponent>(sponza);
        sponza_model.mesh = DFW::DResource::LoadMesh(model_dir + DIR_SLASH + "sponza" + DIR_SLASH + "sponza.gltf");

        // Movable Entity.
        main_entity = em.CreateEntity(*_universe);
        DFW::Transform& transform = em.AddComponent<DFW::TransformComponent>(main_entity);
        transform.translation = glm::vec3(0.0f, 5.0f, 0.0f);
        transform.scale = glm::vec3(2.0f);
        DFW::ModelComponent& model = em.AddComponent<DFW::ModelComponent>(main_entity);
        model.mesh = DFW::Debug_CreateBasicCube();
    }

    void TestStage::OnRemoved()
    {
    }

} // End of namespace ~ DGame.
