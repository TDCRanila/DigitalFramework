#include <ECS/ECSModule.h>

#include <ECS/Managers/ECSystemManager.h>
#include <ECS/Managers/ECSEntityManager.h>
#include <ECS/Managers/ECSComponentManager.h>

namespace DECS 
{
	ECSModule::ECSModule() :
		initialized_(false)
	{ /*EMPTY*/ }

	ECSModule::~ECSModule() { /*EMPTY*/ }

	void ECSModule::InitECS() 
	{
		if (initialized_) 
		{ 
			INFOLOG("ECSModule already initialized");
			return; 
		}

		// Allocate.
		system_manager_		= std::make_unique<ECSystemManager>();
		entity_manager_		= std::make_unique<ECSEntityManager>();
		component_manager_	= std::make_unique<ECSComponentManager>();

		// Init.
		system_manager_->Init();

		initialized_ = true;
	}

	void ECSModule::TerminateECS() 
	{
		if (!initialized_) 
		{ 
			INFOLOG("ECSModule not initialized. No need for termination."); 
			return;
		}

		// Terminate.
		system_manager_->Terminate();
	}

	void ECSModule::UpdateECS() 
	{ 
#if not defined(RELEASE64)
		if (!initialized_) 
		{ 
			ERRORLOG("ECSModule not yet initialized") 
				return; 
		}
#endif

		// Update systems.
		system_manager_->UpdateSystems(); 
	}

} // End of namespace ~ DECS
