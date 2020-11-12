#include <Modules/ECS/Managers/ECSComponentManager.h>

#include <CoreSystems/Logging/Logger.h>

namespace DECS 
{
	ECSComponentManager::ECSComponentManager() :
		entity_manager_(nullptr),
		key_lock_system_(nullptr)
	{ 
		/*EMPTY*/
	}

	ECSComponentManager::~ECSComponentManager() { /*EMPTY*/ }

	void ECSComponentManager::Init(ECSEntityManager* a_entity_manager)
	{
		if (!a_entity_manager)
		{
			DFW_ERRORLOG("Passing an invalid Entity Manager to ComponentManager");
			DFW_ASSERT(false);
		}
		else
		{
			entity_manager_ = a_entity_manager;
		}

		// Allocate.
		key_lock_system_ = std::make_unique<ECSKeyLockSystem>();

		// Generate the component keys.
		key_lock_system_->GenerateComponentKeys();
	}

	void ECSComponentManager::Terminate()
	{

	}

} // End of namespace ~ DECS
