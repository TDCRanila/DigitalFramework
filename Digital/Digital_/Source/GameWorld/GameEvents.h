#pragma once

#include <GameWorld/GameObject.h>

#include <CoreSystems/Events/EventImplementation.h>

namespace DFW
{
    enum class GameEvents
    {
        GameObjectCreatedEvent, GameObjectDestroyedEvent,
        CameraCreatedEvent, CameraDestroyedEvent, CameraNewActiveEvent
    };

#pragma region GameObjectEvents

	class GameObjectCreatedEvent : public Event
	{
	public:
		GameObjectCreatedEvent(GameObject const& a_game_object)
			: game_object(a_game_object)
		{}

		DFW_CONSTRUCT_EVENT(GameEvents, GameObjectCreatedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << "New GameObject Created: ";
			debug_string << game_object.GetID();

			return debug_string.str();
		}

		GameObject game_object;
	};

	class GameObjectDestroyedEvent : public Event
	{
	public:
		GameObjectDestroyedEvent(GameObject const& a_game_object)
			: game_object(a_game_object)
		{}

		DFW_CONSTRUCT_EVENT(GameEvents, GameObjectDestroyedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << "GameObject Destroyed: ";
			debug_string << game_object.GetID();

			return debug_string.str();
		}

		GameObject game_object;
	};

#pragma endregion

} // End of namespace ~ DFW.
