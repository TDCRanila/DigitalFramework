#include <GameWorld/Camera/CameraSystem.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Input/InputKeys.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <GameWorld/Camera/CameraComponent.h>
#include <GameWorld/Camera/CameraEvents.h>
#include <GameWorld/TransformComponent.h>
#include <GameWorld/Transform.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Entity.h>
#include <Modules/ECS/Managers/EntityManager.h>

#include <Defines/MathDefines.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <bgfx/bgfx.h>

namespace DFW
{
    namespace Detail
    {
        constexpr glm::vec3 world_right(1.0f, 0.0f, 0.0f);
        constexpr glm::vec3 world_up(0.0f, 1.0f, 0.0f);
        constexpr glm::vec3 world_front(0.0f, 0.0f, 1.0f);

        bool IsNull(CameraIdentifier const& a_camera_identifier)
        {
            return (a_camera_identifier.registry_name.empty() || a_camera_identifier.camera_name.empty());
        }

    } // End of namespace ~ Detail.

    CameraIdentifier::CameraIdentifier(std::string const& a_registry_name, std::string const& a_camera_name)
        : registry_name(a_registry_name)
        , camera_name(a_camera_name)
    {
    }

    bool CameraIdentifier::operator==(CameraIdentifier const& a_other) const
    {
        return (registry_name == a_other.registry_name) && (camera_name == a_other.camera_name);
    }

    std::size_t CameraIdentifier::HashFunc::operator()(DFW::CameraIdentifier const& a_struct) const
    {
        std::size_t hash(0);
        boost::hash_combine(hash, boost::hash_value(a_struct.registry_name));
        boost::hash_combine(hash, boost::hash_value(a_struct.camera_name));
        return hash;
    }

    CameraSystem::CameraSystem()
        : _active_camera(nullptr)
        , _has_enabled_camera_controls(true)
    {
    }

    CameraComponent& CameraSystem::CreateCamera(DECS::Entity& a_entity, std::string const& a_camera_name)
    {
        if (!a_entity.IsEntityValid())
        {
            DFW_WARNLOG("Trying to create a new camera with name; {}, but entity is invalid.", a_camera_name);
            DFW_ASSERT(a_entity.IsEntityValid());
        }

        if (a_camera_name == "" || a_camera_name.empty())
        {
            DFW_WARNLOG("Trying to create a new camera with an invalid name; {}", a_camera_name);
            DFW_ASSERT(false);
        }

        CameraIdentifier const registration(a_entity.GetRegistry().GetName(), a_camera_name);
        if (auto const& it = registered_cameras.find(registration);
            it != registered_cameras.end())
        {
            return a_entity.GetComponent<CameraComponent>();
        }

        // Create
        CameraComponent& camera = a_entity.AddComponent<CameraComponent>();
        camera.name = a_camera_name;

        // Register
        registered_cameras.emplace(registration, camera);

        // Communicate
        DUID const camera_id        = camera.GetID();
        DUID const camera_owner_id  = camera.GetOwner().GetID();
        ECSEventHandler().Broadcast<CameraCreatedEvent>(camera.name, camera_id, camera_owner_id);

        return camera;
    }

    void CameraSystem::DestroyCamera(CameraIdentifier const& a_camera_identifier)
    {
        if (Detail::IsNull(a_camera_identifier))
            return;   
        
        if (auto const& it = registered_cameras.find(a_camera_identifier); it != registered_cameras.end())
        {
            // Delete camera registration.
            registered_cameras.erase(it);

            // Destroy camera component.
            CameraComponent& found_camera = it->second.get();
            DECS::Entity camera_owner = found_camera.GetOwner();
            camera_owner.DeleteComponent<CameraComponent>();

            // Communicate.
            DUID const camera_id        = found_camera.GetID();
            DUID const camera_owner_id  = found_camera.GetOwner().GetID();
            ECSEventHandler().Broadcast<CameraDestroyedEvent>(found_camera.name, camera_id, camera_owner_id);
        }
    }

    CameraComponent* CameraSystem::GetCamera(CameraIdentifier const& a_camera_identifier) const
    {
        if (Detail::IsNull(a_camera_identifier))
            return nullptr;

        if (auto const& it = registered_cameras.find(a_camera_identifier); it != registered_cameras.end())
        {
            return &it->second.get();
        }

        return nullptr;
    }

    CameraComponent* CameraSystem::GetActiveCamera() const
    {
        return _active_camera;
    }

    void CameraSystem::SetActiveCamera(DECS::Entity& a_entity)
    {
        DFW_ASSERT(a_entity.IsEntityValid());
        SetActiveCamera(a_entity.GetComponent<CameraComponent>());
    }

    void CameraSystem::SetActiveCamera(CameraComponent& a_camera_component)
    {
        _active_camera = &a_camera_component;

        // Communicate.
        Entity const& owner = a_camera_component.GetOwner();
        ECSEventHandler().Broadcast<CameraNewActiveEvent>(CameraIdentifier(owner.GetRegistry().GetName(), a_camera_component.name), owner.GetID());
    }

    void CameraSystem::ChangeCameraProjPerspective(CameraComponent& a_camera_component, float32 a_fov, float32 a_viewport_aspect, ClipSpace a_clip)
    {      
        // TODO
        // Ideally should not call directly to bgfx for this information. 
        // RenderModule should be the layer between the api and the interface.
        if (bgfx::getCaps()->homogeneousDepth)
            a_camera_component.projection = glm::perspectiveLH_NO(a_fov, a_viewport_aspect, a_clip.z_near, a_clip.z_far);
        else
            a_camera_component.projection = glm::perspectiveLH_ZO(a_fov, a_viewport_aspect, a_clip.z_near, a_clip.z_far);
    }

    void CameraSystem::ChangeCameraProjOrthographic(CameraComponent& a_camera_component, float32 a_view_width, float32 a_view_height, ClipSpace a_clip, bool a_center_ortho)
    { 
        float32 const left      = a_center_ortho ? -(a_view_width  * 0.5f) : 0.f;
        float32 const right     = a_center_ortho ?  (a_view_width  * 0.5f) : a_view_width;
        float32 const bottom    = a_center_ortho ? -(a_view_height * 0.5f) : 0.f;
        float32 const top       = a_center_ortho ?  (a_view_height * 0.5f) : a_view_height;

        // TODO
        // Ideally should not call directly to bgfx for this information. 
        // RenderModule should be the layer between the api and the interface.
        if (bgfx::getCaps()->homogeneousDepth)
            a_camera_component.projection = glm::orthoLH_NO(left, right, bottom, top, a_clip.z_near, a_clip.z_far);
        else
            a_camera_component.projection = glm::orthoLH_ZO(left, right, bottom, top, a_clip.z_near, a_clip.z_far);
    }

    void CameraSystem::EnableSimpleCamera(CameraComponent& a_camera_component)
    {
        a_camera_component.has_enabled_six_degrees_rotation = false;

        // TODO Migth want to implement a smoother transition to horizon-locked
        // plane instead and remove the roll component.
        // Reset angles and orientation to nill.
        a_camera_component.orientation = glm::quat();
        a_camera_component.angles = DMath::EulerAngles();
    }

    void CameraSystem::EnableAdvancedCamera(CameraComponent& a_camera_component)
    {
        a_camera_component.has_enabled_six_degrees_rotation = true;
    }

    void CameraSystem::Init()
    {
        _input_management = DFW::CoreService::GetInputManagement();
        DFW_ASSERT(_input_management);
    }

    void CameraSystem::Update(DECS::EntityRegistry& a_registry)
    {
        if (_has_enabled_camera_controls && _active_camera)
        {
            Debug_ToggleCameraMode();
            ControlCamera(*_active_camera, a_registry.ENTT().get<TransformComponent>(_active_camera->GetOwner().GetHandle()));
        }

        for (auto&& [entity, camera_comp, transform_comp] : a_registry.ENTT().view<CameraComponent, TransformComponent>().each())
            UpdateCameraMatrices(camera_comp, transform_comp);
    }

    void CameraSystem::ControlCamera(CameraComponent& a_camera, Transform& a_transform)
    {
        // Directional.

        // TODO Input
        // Implement sensitivity into input/mouse system.
        float32 const mouse_sensitivity(0.2f);
        float32 const mouse_scroll_sensitivity(5.0f);

        glm::vec2 const mouse_delta(_input_management->GetMousePosDelta() * mouse_sensitivity);
        glm::vec2 const mouse_scroll_delta(_input_management->GetMouseScrollDelta() * mouse_scroll_sensitivity);

        if (a_camera.has_enabled_six_degrees_rotation)
        {
            DMath::EulerAngles const angle_delta(mouse_delta.y, mouse_delta.x, mouse_scroll_delta.y);
            a_camera.orientation *= glm::angleAxis(glm::radians(angle_delta.pitch()) , Detail::world_right);
            a_camera.orientation *= glm::angleAxis(glm::radians(angle_delta.yaw())   , Detail::world_up);
            a_camera.orientation *= glm::angleAxis(glm::radians(angle_delta.roll())  , Detail::world_front);
            a_camera.angles       = glm::degrees(glm::eulerAngles(a_camera.orientation));
        }
        else
        {
            a_camera.angles.pitch()  += mouse_delta.y;
            a_camera.angles.yaw()    += mouse_delta.x;

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

        // Camera Speed
        float32 const scroll_offset = _input_management->GetMouseScrollDelta().y;
        float32 const scroll_multiplier(25.0f);
        a_camera.fly_speed += scroll_offset * scroll_multiplier;
        
        float32 const min_fly_speed(1.0f);
        if (a_camera.fly_speed < min_fly_speed)
            a_camera.fly_speed = min_fly_speed;

        float32 const max_fly_speed(10000.f);
        if (a_camera.fly_speed > max_fly_speed)
            a_camera.fly_speed = max_fly_speed;

        // Positional.
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
        a_transform.translation += a_camera.right * (dir.x * dt * a_camera.fly_speed);
        a_transform.translation += a_camera.front * (dir.z * dt * a_camera.fly_speed);
    }

    void CameraSystem::UpdateCameraMatrices(CameraComponent& a_camera, Transform& a_transform)
    {      
        a_camera.right  = glm::rotate(_active_camera->orientation, Detail::world_right);
        a_camera.up     = glm::rotate(_active_camera->orientation, Detail::world_up);
        a_camera.front  = glm::rotate(_active_camera->orientation, Detail::world_front);

        a_camera.view   = glm::inverse(a_transform.GetTransformMatrix() * glm::toMat4(_active_camera->orientation));
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
                EnableSimpleCamera(*_active_camera);
            }
            else
            {
                camera_mode_toggle = true;
                EnableAdvancedCamera(*_active_camera);
            }
        }
    }

} // End of namespace ~ DFW.
