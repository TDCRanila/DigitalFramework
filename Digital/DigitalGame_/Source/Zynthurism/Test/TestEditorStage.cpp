#include <Zynthurism/Test/TestEditorStage.h>

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
        transform.SetTranslation(transform.GetTranslation() + glm::vec3(dir * dt * speed));

        EditorStage::OnUpdate();
    }

    void TestEditorStage::OnRenderImGui()
    {
        EditorStage::OnRenderImGui();
    }

    void TestEditorStage::OnAttached()
    {
        EditorStage::OnAttached();

        // Create Main Camera.
        {
            camera_entity = ECS()->Registry().CreateEntity();
            camera_entity.SetName("camera-main");

            camera_entity.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, 0.0f, 0.0f)));
            DFW::CameraComponent& camera_component = camera_entity.AddComponent<DFW::CameraComponent>();

            DFW::CameraSystem* camera_system =ECS()->SystemManager().GetSystem<DFW::CameraSystem>();
            camera_system->ChangeCameraProjPerspective(camera_component, 60.f, (16.f / 9.f), DFW::ClipSpace(0.1f, 5000.f));
            camera_system->RegisterCamera(camera_component, "camera-main");
            camera_system->SetActiveCamera("camera-main");
            camera_system->EnableCameraControl(camera_component);
        }

        std::string work_dir;
        DFW::DUtility::GetWorkDirectory(work_dir);
        std::string const model_dir(work_dir + DIR_SLASH + "models");

        {
            // XYZ Representation.            
            DFW::Entity xyz = ECS()->Registry().CreateEntity();
            xyz.SetName("xyz-model");

            xyz.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, 0.0f, 0.0f)));

            DFW::ModelComponent& xyz_model = xyz.AddComponent<DFW::ModelComponent>();
            DFW:: FilePath filepath(model_dir + DIR_SLASH + "xyz" + DIR_SLASH + "xyz_10x10m.glb");
            xyz_model.mesh = DFW::DResource::LoadMesh(filepath);
        }

        {
            // Sponza Scene.
            DFW::Entity sponza = ECS()->Registry().CreateEntity();
            sponza.SetName("sponza");
            sponza.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, 0.0f, 0.0f)));

            DFW::ModelComponent& sponza_model = sponza.AddComponent<DFW::ModelComponent>();
            DFW::FilePath filepath(model_dir + DIR_SLASH + "sponza" + DIR_SLASH + "sponza.gltf");
            sponza_model.mesh = DFW::DResource::LoadMesh(filepath);
        }

        {
            // Movable Entity.
            main_entity = ECS()->Registry().CreateEntity();
            main_entity.SetName("main-entity");

            DFW::Transform transform = DFW::Transform(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(2.0f));
            main_entity.AddComponent<DFW::TransformComponent>(transform);
            
            DFW::ModelComponent& model = main_entity.AddComponent<DFW::ModelComponent>();
            model.mesh = DFW::Debug_CreateBasicCube();
        }
    }

    void TestEditorStage::OnRemoved()
    {
        EditorStage::OnRemoved();
    }

} // End of namespace ~ DGame.

