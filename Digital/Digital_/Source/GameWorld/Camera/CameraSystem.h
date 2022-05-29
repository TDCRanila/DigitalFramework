#pragma once

#include <GameWorld/Camera/ClipSpace.h>

#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/ECS/Objects/ECSEntity.h>

#include <CoreSystems/Memory.h>
#include <CoreSystems/DUID.h>

#include <boost/container_hash/hash.hpp>
#include <boost/functional/hash.hpp>

#include <glm/glm.hpp>

#include <functional>
#include <unordered_map>
#include <string>

namespace DFW
{
    // FW Declare.
    struct CameraComponent;
	struct TransformComponent;

	namespace DInput
	{
		class InputManagementSystem;
	}

	struct CameraIdentifier
	{
		CameraIdentifier() = default;
		CameraIdentifier(std::string const& a_universe_name, std::string const& a_camera_name);

		bool operator==(CameraIdentifier const& a_other) const;

		struct HashFunc
		{
			std::size_t operator()(DFW::CameraIdentifier const& a_struct) const;
		};

		std::string universe_name;
		std::string camera_name;
	};

	class CameraSystem : public DECS::System::Registrar<CameraSystem>
    {
    public:
        CameraSystem();
        ~CameraSystem() = default;

		CameraComponent& CreateCamera(DECS::Entity const& a_entity, std::string const& a_camera_name);
		void DestroyCamera(CameraIdentifier const& a_camera_identifier);

		CameraComponent* GetCamera(CameraIdentifier const& a_camera_identifier) const;
		CameraComponent* GetActiveCamera() const;
		void SetActiveCamera(DECS::Entity const& a_entity);
		void SetActiveCamera(CameraComponent& a_camera_component);

		// Camera Utility.
		void ChangeCameraProjPerspective(CameraComponent& a_camera_component, float32 a_fov, float32 a_viewport_aspect, ClipSpace a_clip);
		void ChangeCameraProjOrthographic(CameraComponent& a_camera_component, float32 a_view_width, float32 a_view_height, ClipSpace a_clip, bool a_center_ortho);

		void EnableSimpleCamera(CameraComponent& a_camera_component);
		void EnableAdvancedCamera(CameraComponent& a_camera_component);

    private:
		virtual void Init() override;
		virtual void Update(DECS::Universe& a_universe) override;

		void ControlCamera(CameraComponent& a_camera, TransformComponent& a_transform);
		void UpdateCameraMatrices(CameraComponent& a_camera, TransformComponent& a_transform);

		void Debug_ToggleCameraMode();

	private:
		DFW::DInput::InputManagementSystem* _input_system;

        std::unordered_map<CameraIdentifier, DFW::RefWrap<CameraComponent>, CameraIdentifier::HashFunc> registered_cameras;

		CameraComponent* _active_camera;
		bool _has_enabled_camera_controls;

    };

} // End of namespace ~ DFW.
