#pragma once

#include <DFW/GameWorld/Camera/ClipSpace.h>

#include <DFW/Modules/ECS/System.h>
#include <DFW/Modules/ECS/Entity.h>

#include <DFW/CoreSystems/Memory.h>

#include <unordered_map>
#include <string>

namespace DFW
{
    // FW Declare.
    struct CameraComponent;
	struct Transform;

	namespace DInput
	{
		class InputManagementSystem;
	}

	class CameraSystem final : public DECS::System::Registrar<CameraSystem>
    {
    public:
        CameraSystem();
        ~CameraSystem() = default;

		void RegisterCamera(CameraComponent& a_camera_component, std::string const& a_camera_name);
		void UnregisterCamera(std::string const& a_camera_name);

		CameraComponent* GetCamera(std::string const& a_camera_name) const;
		CameraComponent* GetActiveCamera() const;
		void SetActiveCamera(std::string const& a_camera_name);

		// Camera Utility.
		void ChangeCameraProjPerspective(CameraComponent& a_camera_component, float32 a_fov, float32 a_viewport_aspect, ClipSpace a_clip);
		void ChangeCameraProjOrthographic(CameraComponent& a_camera_component, float32 a_view_width, float32 a_view_height, ClipSpace a_clip, bool a_center_ortho);

		void EnableCameraControl(CameraComponent& a_camera_component);
		void DisableCameraControl(CameraComponent& a_camera_component);

		void EnableSimpleCameraControlMode(CameraComponent& a_camera_component);
		void EnableAdvancedCameraControlMode(CameraComponent& a_camera_component);

    private:
		virtual void Init(DECS::EntityRegistry& a_registry) override;
		virtual void Update(DECS::EntityRegistry& a_registry) override;

		void ControlCamera(CameraComponent& a_camera);

		void Debug_ToggleCameraMode();

	private:
		SharedPtr<DFW::DInput::InputManagementSystem> _input_management;

        std::unordered_map<std::string, CameraComponent*> registered_cameras;
		CameraComponent* _active_camera;

    };

} // End of namespace ~ DFW.
