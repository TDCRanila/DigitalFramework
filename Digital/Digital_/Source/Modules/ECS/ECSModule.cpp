#include <Modules/ECS/ECSModule.h>
		 
#include <Modules/ECS/Managers/ECSEntityManager.h>
#include <Modules/ECS/Managers/ECSystemManager.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/Logging/Logger.h>

namespace DFW
{

	namespace DECS
	{
		ECSModule::ECSModule()
			: _system_manager(nullptr)
			, _entity_manager(nullptr)
			, _event_handler(nullptr)
			, _current_universe(nullptr)
			, _initialized(false)
		{
		}

		ECSModule::~ECSModule() = default;

		void ECSModule::InitECS()
		{
			DFW_ASSERT(!_initialized && "ECSModule already initialized");

			DFW_INFOLOG("Initializing DECS Module.");

			// Allocate.
			_entity_manager = MakeUnique<DECS::EntityManager>();
			_system_manager = MakeUnique<DECS::SystemManager>();
			_event_handler	= MakeUnique<EventDispatcher>();

			// Init.
			_system_manager->Init();

			_initialized = true;
		}

		void ECSModule::TerminateECS()
		{
			DFW_ASSERT(_initialized && "ECSModule not initialized. No need for termination.");

			DFW_INFOLOG("Terminating DECS Module.");

			_current_universe.reset();
			_universes.clear();

			_system_manager->Terminate();

			_initialized = false;
		}

		void ECSModule::UpdateECS()
		{
			DFW_ASSERT(_initialized && "ECSModule not yet initialized");

			// Update systems and events.
			for (auto const& [universe_name, universe_ptr] : _universes)
			{
				_current_universe = universe_ptr;

				_event_handler->ProcessPendingEvents();
				_system_manager->UpdateSystems(*_current_universe);
				_event_handler->ProcessPendingEvents();

				_entity_manager->ManageDeletedEntities(*_current_universe);
			}
		}

		void ECSModule::UpdateECSImGui()
		{
			// Update systems and events.
			for (auto const& [universe_name, universe_ptr] : _universes)
			{
				_system_manager->UpdateSystemsImGui(*universe_ptr);
			}
		}

		SystemManager& ECSModule::SystemManager() const
		{
			return *_system_manager;
		}

		EntityManager& ECSModule::EntityManager() const
		{
			return *_entity_manager;
		}

		EventDispatcher& ECSModule::EventHandler() const
		{
			return *_event_handler;
		}

		Universe* ECSModule::RegisterUniverse(std::string const& a_universe_name)
		{
			if (!_universes.contains(a_universe_name))
				_universes.emplace(a_universe_name, MakeShared<Universe>(DFW::GenerateDUID(), a_universe_name));

			return _universes.at(a_universe_name).get();
		}

		Universe* ECSModule::GetUniverse(std::string const& a_universe_name) const
		{
			if (_universes.contains(a_universe_name))
				return _universes.at(a_universe_name).get();

			return nullptr;
		}

		Universe* ECSModule::CurrentUniverse() const
		{
			DFW_ASSERT(_current_universe);
			return _current_universe.get();
		}

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
