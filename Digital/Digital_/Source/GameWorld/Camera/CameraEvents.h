#pragma once

#include <GameWorld/GameEvents.h>

#include <CoreSystems/Events/EventImplementation.h>

namespace DFW
{
	class CameraCreatedEvent : public Event
	{
	public:
		CameraCreatedEvent(std::string const& a_camera_name, DUID const a_camera_id, DUID const a_camera_owner)
			: camera_name(a_camera_name)
			, camera_id(a_camera_id)
			, camera_owner(a_camera_owner)
		{}

		DFW_CONSTRUCT_EVENT(GameEvents, CameraCreatedEvent);

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

	class CameraDestroyedEvent : public Event
	{
	public:
		CameraDestroyedEvent(std::string const& a_camera_name, DUID a_camera_id, DUID const a_camera_owner)
			: camera_name(a_camera_name)
			, camera_id(a_camera_id)
			, camera_owner(a_camera_owner)
		{}

		DFW_CONSTRUCT_EVENT(GameEvents, CameraDestroyedEvent);

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
