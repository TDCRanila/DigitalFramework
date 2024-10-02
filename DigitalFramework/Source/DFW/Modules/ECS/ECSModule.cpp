#include <DFW/Modules/ECS/ECSModule.h>
		 
#include <DFW/Modules/ECS/Managers/SystemManager.h>
#include <DFW/Modules/ECS/Managers/EntityRegistry.h>

#include <DFW/CoreSystems/Events/EventDispatcher.h>
#include <DFW/CoreSystems/Logging/Logger.h>

namespace DFW
{

	namespace DECS
	{
		ECSModule::ECSModule()
			: _system_manager(nullptr)
			, _event_handler(nullptr)
			, _registry(nullptr)
			, _initialized(false)
		{
		}

		ECSModule::~ECSModule() = default;

		void ECSModule::Init()
		{
			DFW_ASSERT(!_initialized && "ECSModule already initialized");

			DFW_INFOLOG("Initializing DECS Module.");

			// Allocate.
			_event_handler	= MakeUnique<EventDispatcher>();
			_system_manager = MakeUnique<DECS::SystemManager>(this);
			_registry		= MakeUnique<EntityRegistry>(*this);

			_initialized = true;
		}

		void ECSModule::Terminate()
		{
			DFW_ASSERT(_initialized && "ECSModule not initialized. No need for termination.");

			DFW_INFOLOG("Terminating DECS Module.");

			_system_manager->TerminateSystems();

			_registry.reset();

			_system_manager->RemoveSystems(false);

			_initialized = false;
		}

		void ECSModule::UpdateECS()
		{
			DFW_ASSERT(_initialized && "ECSModule not yet initialized");

			// Update systems and events.
			_event_handler->ProcessPendingEvents();
			_system_manager->UpdateSystems();
			_event_handler->ProcessPendingEvents();

			_registry->CleanDestructionMarkedEntities();
		}

		void ECSModule::UpdateECSImGui()
		{
			_system_manager->UpdateSystemsImGui();
		}

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
