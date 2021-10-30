#include <Modules/ECS/ECSModule.h>
		 
#include <Modules/ECS/Managers/ECSEntityManager.h>
#include <Modules/ECS/Managers/ECSystemManager.h>
#include <Modules/ECS/Managers/ECSEventHandler.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <CoreSystems/Logging/Logger.h>

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

	void ECSModule::InitECS() 
	{
		if (_initialized) 
		{ 
			DFW_INFOLOG("ECSModule already initialized");
			return; 
		}

		DFW_INFOLOG("Initializing ECS Module.");

		// Allocate.
		_entity_manager		= new ECSEntityManager();
		_system_manager		= new ECSystemManager();
		_event_handler		= new ECSEventHandler();

		// Init.
		_system_manager->Init();
		_event_handler->Init();

		_initialized = true;
	}

	void ECSModule::TerminateECS() 
	{
		if (!_initialized) 
		{ 
			DFW_INFOLOG("ECSModule not initialized. No need for termination."); 
			return;
		}

		DFW_INFOLOG("Terminating ECS Module.");

		for (auto const& [name, ptr] : _universes)
			delete ptr;

		// Terminate.
		_system_manager->Terminate();
		_event_handler->Terminate();

		delete _entity_manager;
		delete _system_manager;
		delete _event_handler;
	}

	void ECSModule::UpdateECS() 
	{ 
#if !defined(RELEASE64)
		if (!_initialized) 
		{ 
			DFW_ERRORLOG("ECSModule not yet initialized");
			return; 
		}
#endif

		// Update systems and events.
		for (auto [universe_name, universe_ptr] : _universes)
		{
			_current_universe = universe_ptr;
			_event_handler->ProcessPendingEvents();
			_system_manager->UpdateSystems(_current_universe);
			_event_handler->ProcessPendingEvents();

			_entity_manager->ManageDeletedEntities(_current_universe);
		}
	}

	ECSystemManager* const ECSModule::SystemManager() const
	{
		return _system_manager;
	}

	ECSEntityManager* const ECSModule::EntityManager() const
	{
		return _entity_manager;
	}

	ECSEventHandler* const ECSModule::EventHandler() const
	{
		return _event_handler;
	}

	ECSUniverse* ECSModule::RegisterUniverse(std::string const& a_universe_name)
	{
		if (!_universes.contains(a_universe_name))
			return _universes[a_universe_name] = new ECSUniverse();
		else
			return _universes[a_universe_name];
	}

	ECSUniverse* const ECSModule::GetUniverse(std::string const& a_universe_name)
	{
		if (_universes.contains(a_universe_name))
			return _universes[a_universe_name];
		else
			return nullptr;
	}
	
	ECSUniverse* const ECSModule::CurrentUniverse()
	{
		return _current_universe;
	}
	
} // End of namespace ~ DECS
