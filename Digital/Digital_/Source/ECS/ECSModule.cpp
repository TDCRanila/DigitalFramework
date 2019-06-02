#include <ECS/ECSModule.h>

#include <ECS/Managers/ECSystemManager.h>
#include <ECS/Managers/ECSEntityManager.h>
#include <ECS/Managers/ECSComponentManager.h>

namespace DECS 
{
	ECSModule::ECSModule() :
		initialized_(false),
		system_manager_(nullptr),
		entity_manager_(nullptr),
		component_manager_(nullptr)
	{ /*EMPTY*/ }

	ECSModule::~ECSModule() { /*EMPTY*/ }

	void ECSModule::InitECS() 
	{
		if (initialized_) 
		{ 
			INFOLOG("ECSModule already initialized");
			return; 
		}

		INFOLOG("Initializing ECS Module.");

		// Allocate.
		entity_manager_		= new ECSEntityManager();
		system_manager_		= new ECSystemManager();
		component_manager_	= new ECSComponentManager();

		// Init.
		entity_manager_->Init();
		system_manager_->Init(entity_manager_);
		component_manager_->Init(entity_manager_);

		initialized_ = true;
	}

	void ECSModule::TerminateECS() 
	{
		if (!initialized_) 
		{ 
			INFOLOG("ECSModule not initialized. No need for termination."); 
			return;
		}

		INFOLOG("Terminating ECS Module.");

		// Terminate.
		system_manager_->Terminate();
	}

	void ECSModule::UpdateECS() 
	{ 
#if not defined(RELEASE64)
		if (!initialized_) 
		{ 
			ERRORLOG("ECSModule not yet initialized");
			return; 
		}
#endif

		// Update systems.
		entity_manager_->ManageEntities();
		system_manager_->UpdateSystems(); 
	}
	
} // End of namespace ~ DECS
