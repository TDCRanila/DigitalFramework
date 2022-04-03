#include <Zynthurism/Game/TestStage.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Input/InputKeys.h>

#include <Modules/ECS/Managers/ECSEntityManager.h>
#include <Modules/ECS/Managers/ECSystemManager.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <GameWorld/TransformComponent.h>
#include <GameWorld/Camera/CameraComponent.h>
#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Graphics/ModelComponent.h>

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

        transform.rotation.x += 1.0f * dt;
        transform.rotation.y += 0.75f * dt;
        transform.rotation.z += 1.25f * dt;
    }

    void TestStage::OnAttached()
    {
        // Create Universe.
        _universe = ecs->RegisterUniverse("Main");
        
        // Create Simple Cube Field.
        glm::vec3 const extends(4.0f);
        float32 const spacing(4.5f);
        glm::vec3 cube_pos(0.0f);
        for (int32 x = -extends.x; x < extends.x; ++x)
        {
            cube_pos.x = x * spacing;
            for (int32 y = -extends.y; y < extends.y; ++y)
            {
                cube_pos.y = y * spacing;
                for (int32 z = -extends.z; z < extends.z; ++z)
                {
                    cube_pos.z = z * spacing;

                    main_entity = ecs->EntityManager().CreateEntity(*_universe);
                    DFW::TransformComponent& transform = ecs->EntityManager().AddComponent<DFW::TransformComponent>(main_entity);
                    transform.translation = cube_pos;

                    DFW::ModelComponent& model = ecs->EntityManager().AddComponent<DFW::ModelComponent>(main_entity);
                    model.model = DFW::Debug_CreateBasicCube();
                }
            }
        }

        // Create Main Camera.
        camera_entity = ecs->EntityManager().CreateEntity(*_universe);
        ecs->EntityManager().AddComponent<DFW::TransformComponent>(camera_entity);

        DFW::CameraSystem* camera_system = ecs->SystemManager().GetSystem<DFW::CameraSystem>();
        DFW::CameraComponent& camera = camera_system->CreateCamera(camera_entity, "camera-one");
        camera_system->ChangeCameraProjPerspective(camera, 90.f, (16.f/9.f), DFW::ClipSpace(0.1f, 5000.f));
        camera_system->SetActiveCamera(camera_entity);
    }

    void TestStage::OnRemoved()
    {
    }

} // End of namespace ~ DGame.
