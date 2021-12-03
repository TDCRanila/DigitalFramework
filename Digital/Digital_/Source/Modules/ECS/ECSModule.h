#pragma once

#include <memory>
#include <unordered_map>
#include <string>

namespace DFW
{
	// FW Declare.
	class ApplicationInstance;

	namespace DECS
	{
		// FW Declares.
		class SystemManager;
		class EntityManager;
		class EventHandler;
		class Universe;

		class ECSModule final
		{
		public:
			~ECSModule() = default;

			SystemManager* const SystemManager() const;
			EntityManager* const EntityManager() const;
			EventHandler* const EventHandler() const;

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
			DECS::SystemManager* _system_manager;
			DECS::EntityManager* _entity_manager;
			DECS::EventHandler* _event_handler;
			Universe* _current_universe;

			std::unordered_map<std::string, Universe*> _universes;

			bool				_initialized;

		};

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
