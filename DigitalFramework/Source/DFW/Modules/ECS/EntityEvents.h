#pragma once

#include <DFW/Modules/ECS/Entity.h>

#include <DFW/CoreSystems/Events/EventImplementation.h>

namespace DFW
{
	namespace DECS
	{
		enum class EntityEvents
		{
			EntityCreatedEvent, EntityDestroyedEvent,
		};

		class EntityCreatedEvent : public Event
		{
		public:
			EntityCreatedEvent(Entity const& a_entity)
				: entity(a_entity)
			{}

			DFW_CONSTRUCT_EVENT(EntityEvents, EntityCreatedEvent);

			std::string GetDebugString() const override
			{
				std::stringstream debug_string;
				debug_string << GetName();
				debug_string << " - ";
				debug_string << "Entity Created: ";
				debug_string << static_cast<ENTT_ID_TYPE>(entity.GetHandle());
				debug_string << " - ";
				debug_string << entity.GetID();

				return debug_string.str();
			}

			Entity entity;
		};

		class EntityDestroyedEvent : public Event
		{
		public:
			EntityDestroyedEvent(Entity const& a_entity)
				: entity(a_entity)
			{}

			DFW_CONSTRUCT_EVENT(EntityEvents, EntityDestroyedEvent);

			std::string GetDebugString() const override
			{
				std::stringstream debug_string;
				debug_string << GetName();
				debug_string << " - ";
				debug_string << "Entity Destroyed: ";
				debug_string << static_cast<ENTT_ID_TYPE>(entity.GetHandle());
				debug_string << " - ";
				debug_string << entity.GetID();

				return debug_string.str();
			}

			Entity entity;
		};

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
