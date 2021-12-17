#pragma once

#include <memory>
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
		public:
			~ECSModule() = default;

			SystemManager* const SystemManager() const;
			EntityManager* const EntityManager() const;
			EventDispatcher* const EventHandler() const;

			Universe* RegisterUniverse(std::string const& a_universe_name);
			Universe* const GetUniverse(std::string const& a_universe_name);
			Universe* const CurrentUniverse();

		protected:
			friend DFW::ApplicationInstance;

			ECSModule();

			void InitECS();
			void TerminateECS();
			void UpdateECS();
			void UpdateECSImGui();

		private:
			std::unordered_map<std::string, Universe*> _universes;

			DECS::SystemManager*	_system_manager;
			DECS::EntityManager*	_entity_manager;
			EventDispatcher*		_event_handler;
			Universe*				_current_universe;

			bool _initialized;

		};

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
