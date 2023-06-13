#include <GameWorld/Camera/CameraSystem.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Input/InputKeys.h>
#include <CoreSystems/GameClock.h>

#include <GameWorld/Camera/CameraComponent.h>
#include <GameWorld/Camera/CameraEvents.h>
#include <GameWorld/TransformComponent.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Entity.h>

#include <glm/glm.hpp>

#include <bgfx/bgfx.h>

namespace DFW
{
    namespace Detail
    {
        constexpr glm::vec3 world_right(1.0f, 0.0f, 0.0f);
        constexpr glm::vec3 world_up(0.0f, 1.0f, 0.0f);
        constexpr glm::vec3 world_front(0.0f, 0.0f, 1.0f);

        bool IsValidCameraName(std::string const& a_camera_name)
        {
            return !(a_camera_name == "" || a_camera_name.empty());
        }

        void UpdateCameraMatrix(CameraComponent& a_camera, Transform& a_transform)
        {
            a_camera.right  = glm::rotate(a_camera.orientation, Detail::world_right);
            a_camera.up     = glm::rotate(a_camera.orientation, Detail::world_up);
            a_camera.front  = glm::rotate(a_camera.orientation, Detail::world_front);

            a_camera.view   = glm::inverse(a_transform.GetWorldTransformMatrix() * glm::toMat4(a_camera.orientation));
        }

    } // End of namespace ~ Detail.

    CameraSystem::CameraSystem()
        : _active_camera(nullptr)
    {
    }

    void CameraSystem::RegisterCamera(CameraComponent& a_camera_component, std::string const& a_camera_name)
    {
        if (!Detail::IsValidCameraName(a_camera_name))
        {
            DFW_WARNLOG("Trying to register a new camera with an invalid name; {}", a_camera_name);
            DFW_ASSERT(false);
            return;
        }

        // Register if the camera entity isn't already.
        if (auto const& it = registered_cameras.find(a_camera_name); it == registered_cameras.end())
        {
            a_camera_component.name = a_camera_name;
            registered_cameras.emplace(a_camera_name, &a_camera_component);
        }
        else
        {
            DFW_WARNLOG("Trying to register a new camera entity with name; {}, but the name is already registered.", a_camera_name);
            return;
        }

        // Communicate
        DUID const camera_id = a_camera_component.GetID();
        DUID const camera_owner_id  = a_camera_component.GetOwner().GetID();
        ECSEventHandler().Broadcast<CameraRegisteredEvent>(a_camera_name, camera_id, camera_owner_id);

    }

    void CameraSystem::UnregisterCamera(std::string const& a_camera_name)
    {
        if (!Detail::IsValidCameraName(a_camera_name))
            return;   
        
        if (auto const& it = registered_cameras.find(a_camera_name); it != registered_cameras.end())
        {
            // Delete camera registration.
            registered_cameras.erase(it);

            // Communicate.
            CameraComponent const* found_camera = it->second;
            DUID const camera_id = found_camera->GetID();
            DUID const camera_owner_id = found_camera->GetOwner().GetID();
            ECSEventHandler().Broadcast<CameraUnregisteredEvent>(found_camera->name, camera_id, camera_owner_id);
        }
    }

    CameraComponent* CameraSystem::GetCamera(std::string const& a_camera_name) const
    {
        if (!Detail::IsValidCameraName(a_camera_name))
            return nullptr;

        if (auto const& it = registered_cameras.find(a_camera_name); it != registered_cameras.end())
        {
            return it->second;
        }

        return nullptr;
    }

    CameraComponent* CameraSystem::GetActiveCamera() const
    {
        return _active_camera;
    }

    void CameraSystem::SetActiveCamera(std::string const& a_camera_name)
    {
        if (auto const& it = registered_cameras.find(a_camera_name); it != registered_cameras.end())
        {
            CameraComponent* camera_component = it->second;
            _active_camera = camera_component;

            // Communicate.
            DUID const camera_id = camera_component->GetID();
            DUID const camera_owner_id = camera_component->GetOwner().GetID();
            ECSEventHandler().Broadcast<CameraNewActiveEvent>(a_camera_name, camera_id, camera_owner_id);
        }
        else
        {
            DFW_WARNLOG("Failed to activate camera with name; {} as it wasn't registered.", a_camera_name);
        }
    }

    void CameraSystem::ChangeCameraProjPerspective(CameraComponent& a_camera_component, float32 a_fov, float32 a_viewport_aspect, ClipSpace a_clip)
    {      
        // TODO: Ideally should not call directly to bgfx for this information. 
        // RenderModule should be the layer between the api and the interface.
        if (bgfx::getCaps()->homogeneousDepth)
            a_camera_component.projection = glm::perspectiveLH_NO(glm::radians(a_fov), a_viewport_aspect, a_clip.z_near, a_clip.z_far);
        else
            a_camera_component.projection = glm::perspectiveLH_ZO(glm::radians(a_fov), a_viewport_aspect, a_clip.z_near, a_clip.z_far);
    }

    void CameraSystem::ChangeCameraProjOrthographic(CameraComponent& a_camera_component, float32 a_view_width, float32 a_view_height, ClipSpace a_clip, bool a_center_ortho)
    { 
        float32 const left      = a_center_ortho ? -(a_view_width  * 0.5f) : 0.f;
        float32 const right     = a_center_ortho ?  (a_view_width  * 0.5f) : a_view_width;
        float32 const bottom    = a_center_ortho ? -(a_view_height * 0.5f) : 0.f;
        float32 const top       = a_center_ortho ?  (a_view_height * 0.5f) : a_view_height;

        // TODO: Ideally should not call directly to bgfx for this information. 
        // RenderModule should be the layer between the api and the interface.
        if (bgfx::getCaps()->homogeneousDepth)
            a_camera_component.projection = glm::orthoLH_NO(left, right, bottom, top, a_clip.z_near, a_clip.z_far);
        else
            a_camera_component.projection = glm::orthoLH_ZO(left, right, bottom, top, a_clip.z_near, a_clip.z_far);
    }

    void CameraSystem::EnableCameraControl(CameraComponent& a_camera_component)
    {
        a_camera_component.has_enabled_controls = true;
    }

    void CameraSystem::DisableCameraControl(CameraComponent& a_camera_component)
    {
        a_camera_component.has_enabled_controls = false;
    }

    void CameraSystem::EnableSimpleCameraControlMode(CameraComponent& a_camera_component)
    {
        EnableCameraControl(a_camera_component);

        a_camera_component.has_enabled_six_degrees_rotation = false;

        // TODO: Might want to implement a smoother transition to horizon-locked
        // plane instead and remove the roll component.

        // Reset angles and orientation to nill.
        a_camera_component.orientation = glm::quat();
        a_camera_component.angles = DMath::EulerAngles();
    }

    void CameraSystem::EnableAdvancedCameraControlMode(CameraComponent& a_camera_component)
    {
        EnableCameraControl(a_camera_component);

        a_camera_component.has_enabled_six_degrees_rotation = true;
    }

    void CameraSystem::Init(DECS::EntityRegistry& /*a_registry*/)
    {
        _input_management = DFW::CoreService::GetInputManagement();
        DFW_ASSERT(_input_management);
    }

    void CameraSystem::Update(DECS::EntityRegistry& a_registry)
    {
        if (_active_camera && _active_camera->has_enabled_controls)
        {
            Debug_ToggleCameraMode();
            ControlCamera(*_active_camera);
        }

        for (auto&& [entity, camera_comp, transform_comp] : a_registry.ENTT().view<CameraComponent, TransformComponent>().each())
            Detail::UpdateCameraMatrix(camera_comp, transform_comp);
    }

    void CameraSystem::ControlCamera(CameraComponent& a_camera)
    {
        if (_input_management->IsKeyDown(DFW::DInput::DMouse::BUTTON_RIGHT))
        {
            // TODO: Input: Implement sensitivity into input/mouse system.
            float32 const mouse_sensitivity(0.2f);
            float32 const mouse_scroll_sensitivity(5.0f);

            glm::vec2 const mouse_delta(_input_management->GetMousePosDelta() * mouse_sensitivity);
            glm::vec2 const mouse_scroll_delta(_input_management->GetMouseScrollDelta() * mouse_scroll_sensitivity);

            // Camera Orientation.
            if (a_camera.has_enabled_six_degrees_rotation)
            {
                DMath::EulerAngles const angle_delta(mouse_delta.y, mouse_delta.x, mouse_scroll_delta.y);
                a_camera.orientation *= glm::angleAxis(glm::radians(angle_delta.pitch()), Detail::world_right);
                a_camera.orientation *= glm::angleAxis(glm::radians(angle_delta.yaw()), Detail::world_up);
                a_camera.orientation *= glm::angleAxis(glm::radians(angle_delta.roll()), Detail::world_front);
                a_camera.angles = glm::degrees(glm::eulerAngles(a_camera.orientation));
            }
            else
            {
                a_camera.angles.pitch() += mouse_delta.y;
                a_camera.angles.yaw() += mouse_delta.x;

                // Prevent Gimbal Lock.
                if (a_camera.angles.pitch() > 90.f)
                    a_camera.angles.pitch() = 90.f;
                if (a_camera.angles.pitch() < -90.f)
                    a_camera.angles.pitch() = -90.f;

                // Wrap Yaw Angle.
                if (a_camera.angles.yaw() > 180.f)
                    a_camera.angles.yaw() = -180.f;
                if (a_camera.angles.yaw() < -180.f)
                    a_camera.angles.yaw() = 180.f;

                glm::vec3 const pitch_yaw_angles(a_camera.angles.pitch(), a_camera.angles.yaw(), 0.0f);
                glm::quat const orientation_offset(glm::radians(pitch_yaw_angles));
                a_camera.orientation = orientation_offset;
            }

            a_camera.orientation = glm::normalize(a_camera.orientation);
        }

        // Camera Speed.
        float32 const scroll_offset = _input_management->GetMouseScrollDelta().y;
        float32 const scroll_multiplier(25.0f);
        a_camera.fly_speed += scroll_offset * scroll_multiplier;
        
        float32 const min_fly_speed(1.0f);
        if (a_camera.fly_speed < min_fly_speed)
            a_camera.fly_speed = min_fly_speed;

        float32 const max_fly_speed(10000.f);
        if (a_camera.fly_speed > max_fly_speed)
            a_camera.fly_speed = max_fly_speed;

        // Camera Positioning.
        glm::vec3 dir(0.0f);
        if (_input_management->IsKeyDown(DFW::DInput::DKey::LEFT))
            dir.x += -1.0f;
        if (_input_management->IsKeyDown(DFW::DInput::DKey::RIGHT))
            dir.x += 1.0f;
        if (_input_management->IsKeyDown(DFW::DInput::DKey::UP))
            dir.z += 1.0f;
        if (_input_management->IsKeyDown(DFW::DInput::DKey::DOWN))
            dir.z += -1.0f;

        TimeUnit const dt = DFW::CoreService().GetGameClock()->GetLastFrameDeltaTime();
        Transform& transform = a_camera.GetOwner().GetComponent<TransformComponent>();
        transform.Translate(a_camera.right * (dir.x * dt * a_camera.fly_speed));
        transform.Translate(a_camera.front * (dir.z * dt * a_camera.fly_speed));
    }

    void CameraSystem::Debug_ToggleCameraMode()
    {
        if (_input_management->IsKeyPressed(DInput::DKey::F3))
        {
            DFW_ASSERT(_active_camera);
            static bool camera_mode_toggle(false);
            if (camera_mode_toggle)
            {
                camera_mode_toggle = false;
                EnableSimpleCameraControlMode(*_active_camera);
            }
            else
            {
                camera_mode_toggle = true;
                EnableAdvancedCameraControlMode(*_active_camera);
            }
        }
    }

} // End of namespace ~ DFW.
