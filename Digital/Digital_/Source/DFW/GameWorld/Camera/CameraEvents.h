#pragma once

#include <GameWorld/GameEvents.h>
#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Camera/CameraEvents.h>

#include <CoreSystems/Events/EventImplementation.h>

namespace DFW
{
	class CameraRegisteredEvent : public Event
	{
	public:
		CameraRegisteredEvent(std::string const& a_camera_name, DUID a_camera_id, DUID a_camera_owner)
			: camera_name(a_camera_name)
			, camera_id(a_camera_id)
			, camera_owner(a_camera_owner)
		{}

		DFW_CONSTRUCT_EVENT(GameEvents, CameraRegisteredEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << "Camera: ";
			debug_string << camera_name;
			debug_string << "[";
			debug_string << camera_id;
			debug_string << "]";

			return debug_string.str();
		}

		std::string camera_name;
		DUID camera_id;
		DUID camera_owner;
	};

	class CameraUnregisteredEvent : public Event
	{
	public:
		CameraUnregisteredEvent(std::string const& a_camera_name, DUID a_camera_id, DUID a_camera_owner)
			: camera_name(a_camera_name)
			, camera_id(a_camera_id)
			, camera_owner(a_camera_owner)
		{}

		DFW_CONSTRUCT_EVENT(GameEvents, CameraUnregisteredEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << "Camera: ";
			debug_string << camera_name;
			debug_string << "[";
			debug_string << camera_id;
			debug_string << "]";

			return debug_string.str();
		}

		std::string camera_name;
		DUID camera_id;
		DUID camera_owner;
	};

	class CameraNewActiveEvent : public Event
	{
	public:
		CameraNewActiveEvent(std::string const& a_camera_name, DUID a_camera_id, DUID a_camera_owner)
			: camera_name(a_camera_name)
			, camera_id(a_camera_id)
			, camera_owner(a_camera_owner)
		{}

		DFW_CONSTRUCT_EVENT(GameEvents, CameraNewActiveEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << "Camera: ";
			debug_string << camera_name;
			debug_string << "[";
			debug_string << camera_id;
			debug_string << "]";

			return debug_string.str();
		}

		std::string camera_name;
		DUID camera_id;
		DUID camera_owner;
	};

} // End of namespace ~ DFW.
