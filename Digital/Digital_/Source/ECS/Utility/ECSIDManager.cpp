#include <ECS/Utility/ECSIDManager.h>

namespace DECS {

	// Static ID Variables Initialization.
	SystemID ECSIDManager::next_system_id_			= 0;
	EntityID ECSIDManager::next_entity_id_			= 0;
	ComponentID ECSIDManager::next_component_id_	= 0;

	ECSIDManager::ECSIDManager() { /*EMPTY*/ }

	ECSIDManager::~ECSIDManager() { /*EMPTY*/ }

	void ECSIDManager::ResetIDCounters() {
		next_system_id_		= 0;
		next_entity_id_		= 0;
		next_component_id_	= 0;
	}

	SystemID ECSIDManager::GetNextSystemID()		{ return next_system_id_++;		}
	EntityID ECSIDManager::GetNextEntityID()		{ return next_entity_id_++;		}
	ComponentID ECSIDManager::GetNextComponentID()	{ return next_component_id_++;	}

} // End of namespace ~ DECS