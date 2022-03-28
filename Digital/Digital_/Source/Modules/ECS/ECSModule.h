#pragma once

#include <CoreSystems/Memory.h>

#include <unordered_map>
#include <string>

namespace DFW
{
	// FW Declare.
	class ApplicationInstance;
	class EventDispatcher;

	namespace DECS
	{
		// FW Declares.
		class SystemManager;
		class EntityManager;
		class Universe;

		class ECSModule final
		{
		private:
			friend DFW::ApplicationInstance;

		public:
			ECSModule();
			~ECSModule();

			SystemManager& SystemManager() const;
			EntityManager& EntityManager() const;
			EventDispatcher& EventHandler() const;

			Universe* RegisterUniverse(std::string const& a_universe_name);
			Universe* GetUniverse(std::string const& a_universe_name) const;
			Universe* CurrentUniverse() const;

		private:
			void InitECS();
			void TerminateECS();
			void UpdateECS();
			void UpdateECSImGui();

		private:
			std::unordered_map<std::string, SharedPtr<Universe>> _universes;

			UniquePtr<DFW::DECS::SystemManager>	_system_manager;
			UniquePtr<DFW::DECS::EntityManager>	_entity_manager;
			UniquePtr<EventDispatcher>			_event_handler;
			SharedPtr<Universe>					_current_universe;

			bool _initialized;

		};

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
