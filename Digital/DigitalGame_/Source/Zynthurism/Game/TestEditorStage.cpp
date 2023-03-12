#include <Zynthurism/Game/TestEditorStage.h>

#include <GameWorld/TransformComponent.h>
#include <GameWorld/Camera/CameraComponent.h>
#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Graphics/ModelComponent.h>
#include <GameWorld/Graphics/SpriteComponent.h>

#include <Modules/ECS/Managers/SystemManager.h>
#include <Modules/Resource/Mesh/MeshLoader.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Input/InputKeys.h>

#include <Utility/FileSystemUtility.h>

namespace DGame
{

    TestEditorStage::TestEditorStage(std::string const& a_stage_name, bool a_start_disabled)
        : DFW::DEditor::EditorStage(a_stage_name, a_start_disabled)
    {
    }

    void TestEditorStage::OnUpdate()
    {
        // Move the main entity.
        glm::vec3 dir(0.0f);
        if (DFW::CoreService::GetInputManagement()->IsKeyDown(DFW::DInput::DKey::A))
            dir.x -= 1.0f;
        if (DFW::CoreService::GetInputManagement()->IsKeyDown(DFW::DInput::DKey::D))
            dir.x += 1.0f;

        if (DFW::CoreService::GetInputManagement()->IsKeyDown(DFW::DInput::DKey::W))
            dir.y += 1.0f;
        if (DFW::CoreService::GetInputManagement()->IsKeyDown(DFW::DInput::DKey::S))
            dir.y -= 1.0f;

        if (DFW::CoreService::GetInputManagement()->IsKeyDown(DFW::DInput::DKey::Q))
            dir.z += 1.0f;
        if (DFW::CoreService::GetInputManagement()->IsKeyDown(DFW::DInput::DKey::E))
            dir.z -= 1.0f;

        DFW::TimeUnit const dt = DFW::CoreService::GetGameClock()->GetLastFrameDeltaTime();
        float32 const speed = 10.0f;

        DFW::Transform& transform = main_entity.GetComponent<DFW::TransformComponent>();
        transform.translation.x += dir.x * dt * speed;
        transform.translation.y += dir.y * dt * speed;
        transform.translation.z += dir.z * dt * speed;

        EditorStage::OnUpdate();
    }

    void TestEditorStage::OnRenderImGui()
    {
        EditorStage::OnRenderImGui();
    }

    void TestEditorStage::OnAttached()
    {
        EditorStage::OnAttached();

        DFW::SharedPtr<DFW::GameWorld>const & game_world = GameWorld();

        // Create Main Camera.
        {
            DFW::SpawnInfo camera_spawn_info;
            camera_spawn_info.name = "camera";
            camera_spawn_info.transform = DFW::Transform(glm::vec3(0.0f, 0.0f, 0.0f));

            camera_entity = game_world->SpawnGameObject(camera_spawn_info);

            DFW::CameraSystem* camera_system = game_world->GetECS().SystemManager().GetSystem<DFW::CameraSystem>();
            DFW::CameraComponent& camera = camera_system->CreateCamera(camera_entity, "camera-one");
            camera_system->ChangeCameraProjPerspective(camera, 90.f, (16.f / 9.f), DFW::ClipSpace(0.1f, 5000.f));
            camera_system->SetActiveCamera(camera_entity);
        }

        std::string work_dir;
        DFW::DUtility::GetWorkDirectory(work_dir);
        std::string const model_dir(work_dir + DIR_SLASH + "models");

        {
            // XYZ Representation.
            DFW::SpawnInfo xyz_spawn_info;
            xyz_spawn_info.name = "xyz-model";
            xyz_spawn_info.transform = DFW::Transform(glm::vec3(0.0f, 0.0f, 0.0f));
            
            DFW::Entity xyz = game_world->SpawnGameObject(xyz_spawn_info);

            DFW::ModelComponent& xyz_model = xyz.AddComponent<DFW::ModelComponent>();
            xyz_model.mesh = DFW::DResource::LoadMesh(model_dir + DIR_SLASH + "xyz" + DIR_SLASH + "xyz_10x10m.glb");
        }

        {
            // Sponza Scene.
            DFW::SpawnInfo sponza_spawn_info;
            sponza_spawn_info.name = "sponza";
            sponza_spawn_info.transform = DFW::Transform(glm::vec3(0.0f, 0.0f, 0.0f));

            DFW::Entity sponza = game_world->SpawnGameObject(sponza_spawn_info);

            DFW::ModelComponent& sponza_model = sponza.AddComponent<DFW::ModelComponent>();
            sponza_model.mesh = DFW::DResource::LoadMesh(model_dir + DIR_SLASH + "sponza" + DIR_SLASH + "sponza.gltf");
        }

        {
            // Movable Entity.
            DFW::SpawnInfo main_entity_spawn_info;
            main_entity_spawn_info.name = "main-entity";
            main_entity_spawn_info.transform = DFW::Transform(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(2.0f));

            main_entity = game_world->SpawnGameObject(main_entity_spawn_info);

            DFW::ModelComponent& model = main_entity.AddComponent<DFW::ModelComponent>();
            model.mesh = DFW::Debug_CreateBasicCube();
        }
    }

    void TestEditorStage::OnRemoved()
    {
        EditorStage::OnRemoved();
    }

} // End of namespace ~ DGame.

