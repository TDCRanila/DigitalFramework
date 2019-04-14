#include <ECS/ECSModule.h>

#include <ECS/Managers/ECSystemManager.h>
#include <ECS/Managers/ECSEntityManager.h>
#include <ECS/Managers/ECSComponentManager.h>
#include <ECS/Utility/ECSIDManager.h>

namespace DECS 
{

	ECSModule::ECSModule() :
		_initialized(false)
	{ /*EMPTY*/ }

	ECSModule::~ECSModule() { /*EMPTY*/ }

	void ECSModule::InitECS() 
	{
		if (_initialized) { INFOLOG("ECSModule already initialized") return; }

		// Allocate.
		_system_manager		= std::make_unique<ECSystemManager>();
		_entity_manager		= std::make_unique<ECSEntityManager>();
		_component_manager	= std::make_unique<ECSComponentManager>();

		// Init.
		_system_manager->Init();

		_initialized = true;
	}

	void ECSModule::TerminateECS() 
	{
		if (!_initialized) { INFOLOG("ECSModule not initialized. No need for termination.") return; }

		// Terminate.
		_system_manager->Terminate();

	}

	void ECSModule::UpdateECS() 
	{ 
#if not defined(RELEASE64)
		if (!_initialized) { ERRORLOG("ECSModule not yet initialized") return; }
#endif

		// Update systems.
		_system_manager->UpdateSystems(); 
	}

} // End of namespace ~ DECS
