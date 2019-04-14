#include <ECS/Utility/ECSIDManager.h>

namespace DECS 
{
	// Static ID Variables Initialization.
	SystemID ECSIDManager::next_system_id_			= UNASSIGNED_SYSTEM_ID;
	EntityID ECSIDManager::next_entity_id_			= UNASSIGNED_ENTITY_ID;
	ComponentID ECSIDManager::next_component_id_	= UNASSIGNED_COMPONENT_ID;

	ECSIDManager::ECSIDManager() { /*EMPTY*/ }

	ECSIDManager::~ECSIDManager() { /*EMPTY*/ }

	void ECSIDManager::ResetIDCounters() 
	{
		next_system_id_		= UNASSIGNED_SYSTEM_ID;
		next_entity_id_		= UNASSIGNED_ENTITY_ID;
		next_component_id_	= UNASSIGNED_COMPONENT_ID;
	}

	SystemID ECSIDManager::GetNextSystemID()		{ return ++next_system_id_;		}
	EntityID ECSIDManager::GetNextEntityID()		{ return ++next_entity_id_;		}
	ComponentID ECSIDManager::GetNextComponentID()	{ return ++next_component_id_;	}

} // End of namespace ~ DECS
