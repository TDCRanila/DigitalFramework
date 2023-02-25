#pragma once

#include <CoreSystems/Memory.h>

#include <unordered_map>
#include <string>

namespace DFW
{
	// FW Declare.
	class EventDispatcher;

	namespace DECS
	{
		// FW Declares.
		class SystemManager;
		class EntityManager;
		class Universe;

		class ECSModule final
		{
		public:
			ECSModule();
			~ECSModule();

			void Init();
			void Terminate();
			void UpdateECS();
			void UpdateECSImGui();

		public:
			SystemManager& SystemManager() const;
			EntityManager& EntityManager() const;
			EventDispatcher& EventHandler() const;

			Universe& GetUniverse() const;

		private:
			UniquePtr<DFW::DECS::SystemManager>	_system_manager;
			UniquePtr<DFW::DECS::EntityManager>	_entity_manager;
			UniquePtr<EventDispatcher>			_event_handler;

			UniquePtr<Universe> _current_universe;
			bool _initialized;

		};

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
