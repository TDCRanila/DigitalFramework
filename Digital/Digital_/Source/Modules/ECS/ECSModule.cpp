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

		void ECSModule::Init()
		{
			DFW_ASSERT(!_initialized && "ECSModule already initialized");

			DFW_INFOLOG("Initializing DECS Module.");

			// Allocate.
			_entity_manager = MakeUnique<DECS::EntityManager>();
			_event_handler	= MakeUnique<EventDispatcher>();
			_system_manager = MakeUnique<DECS::SystemManager>(this);
			_current_universe = MakeUnique<Universe>("main");

			_initialized = true;
		}

		void ECSModule::Terminate()
		{
			DFW_ASSERT(_initialized && "ECSModule not initialized. No need for termination.");

			DFW_INFOLOG("Terminating DECS Module.");

			_current_universe.reset();

			_system_manager->Terminate();

			_initialized = false;
		}

		void ECSModule::UpdateECS()
		{
			DFW_ASSERT(_initialized && "ECSModule not yet initialized");

			// Update systems and events.
			_event_handler->ProcessPendingEvents();
			_system_manager->UpdateSystems(*_current_universe);
			_event_handler->ProcessPendingEvents();

			_entity_manager->ManageDeletedEntities(*_current_universe);
		}

		void ECSModule::UpdateECSImGui()
		{

			_system_manager->UpdateSystemsImGui(*_current_universe);
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

		Universe& ECSModule::GetUniverse() const
		{
			DFW_ASSERT(_current_universe);
			return *_current_universe;
		}

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
