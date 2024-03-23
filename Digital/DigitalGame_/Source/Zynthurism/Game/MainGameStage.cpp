#include <Zynthurism/Game/MainGameStage.h>

#include <Zynthurism/Game/Level.h>

#include <GameWorld/TransformComponent.h>
#include <GameWorld/Camera/CameraComponent.h>
#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Graphics/ModelComponent.h>
#include <GameWorld/Graphics/SpriteComponent.h>
#include <GameWorld/Physics/PhysicsComponent.h>

#include <GameWorld/TransformSystem.h>
#include <GameWorld/Editor/GameViewport.h>
#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Graphics/RenderSystem.h>
#include <GameWorld/Graphics/SpriteSystem.h>
#include <GameWorld/Physics/PhysicsSystem.h>
#include <GameWorld/Physics/PhysicsLayers.h>
#include <GameWorld/Graphics/DebugRenderSystem.h>

#include <GameWorld/Controller/ControllerSystem.h>
#include <GameWorld/Character/MovementSystem.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/SystemManager.h>
#include <Modules/Resource/Mesh/MeshLoader.h>
#include <CoreSystems/CoreServices.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Input/InputKeys.h>

#include <Utility/FilePath.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Character/Character.h>>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace DGame
{
    MainGameStage::MainGameStage(std::string const& a_stage_name, bool a_start_disabled)
        : DFW::StageBase(a_stage_name, a_start_disabled)
    {
    }

    void MainGameStage::OnUpdate()
    {
        _ecs_module->UpdateECS();

        auto game_clock = DFW::CoreService::GetGameClock();
        
        auto input = DFW::CoreService::GetInputManagement();
        if (input->IsKeyDown(DFW::DInput::DKey::W))
            _player_controller->HandleDirectionalMoveInput(glm::vec3(0.0f, 0.0f, 1.0f));
        if (input->IsKeyDown(DFW::DInput::DKey::S))
            _player_controller->HandleDirectionalMoveInput(glm::vec3(0.0f, 0.0f, -1.0f));
        if (input->IsKeyDown(DFW::DInput::DKey::A))
            _player_controller->HandleDirectionalMoveInput(glm::vec3(-1.0f, 0.0f, 0.0f));
        if (input->IsKeyDown(DFW::DInput::DKey::D))
            _player_controller->HandleDirectionalMoveInput(glm::vec3(1.0f, 0.0f, 0.0f));

        if (input->IsKeyDown(DFW::DInput::DKey::SPACE))
            _player_controller->Jump();

        if (_xyz)
        {
            auto dt = DFW::CoreService::GetGameClock()->GetLastFrameDeltaTime();
            auto st = DFW::CoreService::GetGameClock()->GetElapsedTimeInSeconds();

            DFW::TransformComponent& transform = _xyz.GetComponent<DFW::TransformComponent>();
            glm::vec3 rotate_speed(cos(st), 1.0f, sin(st));
            transform.RotateEuler(rotate_speed * dt);
        }
    }

    void MainGameStage::OnRenderImGui()
    {
        _ecs_module->UpdateECSImGui();
    }

    void MainGameStage::OnAttached()
    {
        // Allocate
        _ecs_module = DFW::MakeUnique<DFW::DECS::ECSModule>();

        // Init Systems
        _ecs_module->Init();

        DFW::DECS::ECSModule& ecs = *_ecs_module;
        auto& render_system = ecs.SystemManager().AddSystem<DFW::RenderSystem>();
        auto& debug_render_system = ecs.SystemManager().AddSystem<DFW::DebugRenderSystem>();
        auto& sprite_system = ecs.SystemManager().AddSystem<DFW::SpriteSystem>();
        
        auto& controller_system = ecs.SystemManager().AddSystem<DFW::ControllerSystem>();
        auto& movement_system = ecs.SystemManager().AddSystem<DFW::MovementSystem>();

        auto& camera_system = ecs.SystemManager().AddSystem<DFW::CameraSystem>();

        auto& transform_system = ecs.SystemManager().AddSystem<DFW::TransformSystem>();
        
        auto& physics_system = ecs.SystemManager().AddSystem<DFW::PhysicsSystem>();
        physics_system.Debug_EnableDebugDraw();
        
        render_system.ExecuteAfter(camera_system);
        debug_render_system.ExecuteAfter(camera_system);
        sprite_system.ExecuteAfter(camera_system);

        transform_system.ExecuteAfter(camera_system);
        camera_system.ExecuteAfter(physics_system);
        physics_system.ExecuteAfter(movement_system);
        movement_system.ExecuteAfter(controller_system);

        ecs.SystemManager().CalculateSystemDependencies();

        {
            DFW::Entity physics_object = ecs.Registry().CreateEntity();
            physics_object.AddComponent<DFW::TransformComponent>(glm::vec3(0.0f, 10.0f, 0.0f));
            physics_object.AddComponent<DFW::ModelComponent>(DFW::Debug_CreateBasicCube(), nullptr);
            physics_object.AddComponent<DFW::RigidBodyComponent>(ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(0.0f, 10.0f, 0.f)), glm::vec3(1.0f), JPH::EMotionType::Dynamic));
        }

        {
            DFW::Entity physics_object = ecs.Registry().CreateEntity();
            physics_object.AddComponent<DFW::TransformComponent>(glm::vec3(0.0f, 15.0f, 0.0f));
            physics_object.AddComponent<DFW::ModelComponent>(DFW::Debug_CreateBasicCube(), nullptr);
            physics_object.AddComponent<DFW::RigidBodyComponent>(ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(0.0f, 15.0f, 0.f)), glm::vec3(1.0f), JPH::EMotionType::Dynamic));
        }

        {
            DFW::Entity physics_object = ecs.Registry().CreateEntity();
            physics_object.AddComponent<DFW::TransformComponent>(glm::vec3(0.0f, 25.0f, 0.0f));
            physics_object.AddComponent<DFW::ModelComponent>(DFW::Debug_CreateBasicCube(), nullptr);
            physics_object.AddComponent<DFW::RigidBodyComponent>(ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(0.0f, 25.0f, 0.f)), glm::vec3(1.0f), JPH::EMotionType::Dynamic));
        }

        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(0.0f, 0.0f, 25.f)), glm::vec3(2.0f), JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(0.0f, 2.0f, 25.f)), glm::vec3(2.0f), JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(0.0f, -2.0f, 25.f)), glm::vec3(2.0f), JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(2.0f, 0.0f, 25.f)), glm::vec3(2.0f), JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(-2.0f, 0.0f, 10.f)), glm::vec3(2.0f), JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(0.0f, 0.0f, 15.f)), glm::vec3(2.0f), JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateBoxRigidBody(DFW::Transform(glm::vec3(0.0f, 0.0f, 20.f)), glm::vec3(2.0f), JPH::EMotionType::Dynamic);

        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateSphereRigidBody(DFW::Transform(glm::vec3(0.0f, 0.0f, 25.f)), 1.0f, JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateSphereRigidBody(DFW::Transform(glm::vec3(0.0f, 2.0f, 25.f)), 2.0f, JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateSphereRigidBody(DFW::Transform(glm::vec3(0.0f, -2.0f, 25.f)), 4.0f, JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateSphereRigidBody(DFW::Transform(glm::vec3(2.0f, 0.0f, 10.f)), 0.5f, JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateSphereRigidBody(DFW::Transform(glm::vec3(-2.0f, 0.0f, 10.f)), 1.0f, JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateSphereRigidBody(DFW::Transform(glm::vec3(0.0f, 0.0f, 15.f)), 0.5f, JPH::EMotionType::Dynamic);
        ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateSphereRigidBody(DFW::Transform(glm::vec3(0.0f, 0.0f, 20.f)), 1.0f, JPH::EMotionType::Dynamic);

        // Map
        DFW::FilePath map_file("maps/default-test/default-test-large.map");
        DFW::SpawnInfo spawn_info;
        spawn_info.transform = DFW::Transform({0.0f, 0.0f, 0.0f});
        _level = SpawnLevel(map_file, spawn_info, ecs);

        // Create Main Camera.
        {
            _camera_entity = ecs.Registry().CreateEntity();
            _camera_entity.SetName("camera-main");
            _camera_entity.SetType<"Camera">();

            _camera_entity.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, 0.0f, 0.0f)));

            DFW::CameraComponent& camera_component = _camera_entity.AddComponent<DFW::CameraComponent>();

            DFW::CameraSystem* camera_system = _ecs_module->SystemManager().GetSystem<DFW::CameraSystem>();
            camera_system->ChangeCameraProjPerspective(camera_component, 60.f, (16.f / 9.f), DFW::ClipSpace(0.1f, 5000.f));
            camera_system->RegisterCamera(camera_component, "camera-main");
            camera_system->SetActiveCamera("camera-main");
            camera_system->EnableCameraControl(camera_component);
        }

        // Player.
        {
            _player = ecs.Registry().CreateEntity();
            _player.SetName("player");
            _player.SetType<"Player">();

            DFW::Transform init_transform(glm::vec3(0.0f, 10.0f, 0.0f));
            _player.AddComponent<DFW::TransformComponent>(init_transform);
            
            DFW::ModelComponent& model = _player.AddComponent<DFW::ModelComponent>();
            model.mesh = DFW::Debug_CreateBasicCube();

            JPH::CharacterSettings character_settings;;
            character_settings.mMass = 1000.f;
            character_settings.mMaxSlopeAngle = DFW::DMath::DegToRad(45.0f);
            character_settings.mLayer = DFW::PhysicsLayers::MOVING;
            character_settings.mShape = new JPH::BoxShape(JPH::RVec3(2.0f, 2.0f, 2.0));
            character_settings.mFriction = 0.5f;
            //character_settings.mSupportingVolume = Plane(Vec3::sAxisY(), -cCharacterRadiusStanding); // Accept contacts that touch the lower sphere of the capsule

            DFW::CharacterComponent& character = _player.AddComponent<DFW::CharacterComponent>();
            character.character = ecs.SystemManager().GetSystem<DFW::PhysicsSystem>()->CreateCharacter(init_transform, character_settings);

            DFW::RigidBodyComponent& rigid_body = _player.AddComponent<DFW::RigidBodyComponent>();
            rigid_body.body_id = character.character->GetBodyID();

            _player_controller = DFW::MakeShared<DFW::FPSCharacterController>();
            controller_system.RegisterController(_player_controller);
            _player_controller->PossesCharacter(_player);

            _player.AddChild(_camera_entity);

        }

        {
            // Sponza Scene.
            DFW::Entity sponza = _ecs_module->Registry().CreateEntity();
            sponza.SetName("sponza");
            sponza.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, -20.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

            DFW::FilePath filepath(std::string("models") + DIR_SLASH + "sponza" + DIR_SLASH + "sponza.gltf");
            DFW::ModelComponent& sponza_model = sponza.AddComponent<DFW::ModelComponent>(DFW::DResource::LoadMesh(filepath), nullptr);
        }

        {
            // XYZ Representation.            
            _xyz = _ecs_module->Registry().CreateEntity();
            DFW::Entity& xyz = _xyz;
            xyz.SetName("xyz-model");

            xyz.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, 32.0f, 0.0f)));

            DFW::ModelComponent& xyz_model = xyz.AddComponent<DFW::ModelComponent>();
            DFW::FilePath filepath(std::string("models") + DIR_SLASH + "xyz" + DIR_SLASH + "xyz_10x10m.glb");
            xyz_model.mesh = DFW::DResource::LoadMesh(filepath);

            //xyz.AddChild(_camera_entity);

            {
                //X+
                DFW::Entity x = _ecs_module->Registry().CreateEntity();

                x.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(16.0f, 0.0f, 0.0f)));

                DFW::ModelComponent& xmodel = x.AddComponent<DFW::ModelComponent>();
                xmodel.mesh = DFW::Debug_CreateBasicCube(DFW::ColourRGBA::Red);
                
                xyz.AddChild(x);
            }
            {
                //Y+
                DFW::Entity y = _ecs_module->Registry().CreateEntity();

                y.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, 16.0f, 0.0f)));

                DFW::ModelComponent& ymodel = y.AddComponent<DFW::ModelComponent>();
                ymodel.mesh = DFW::Debug_CreateBasicCube(DFW::ColourRGBA::Green);

                xyz.AddChild(y);
            }
            {
                //Z+
                DFW::Entity z = _ecs_module->Registry().CreateEntity();

                z.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, 0.0f, 16.0f)));

                DFW::ModelComponent& zmodel = z.AddComponent<DFW::ModelComponent>();
                zmodel.mesh = DFW::Debug_CreateBasicCube(DFW::ColourRGBA::Blue);

                xyz.AddChild(z);
            }
        }
    }

    void MainGameStage::OnRemoved()
    {
        _ecs_module->Terminate();
    }

} // End of namespace ~ DGame.

