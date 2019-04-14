#pragma once

#include <Defines/Defines.h>

namespace DECS 
{

	typedef int64 SystemID;
	typedef int64 EntityID;
	typedef int64 ComponentID;

	const int64 UNASSIGNED_SYSTEM_ID	= -1;
	const int64 UNASSIGNED_ENTITY_ID	= -1;
	const int64 UNASSIGNED_COMPONENT_ID	= -1;

	class ECSIDManager final 
	{
	public:
		ECSIDManager();
		~ECSIDManager();

		static void ResetIDCounters();

		static SystemID GetNextSystemID();
		static EntityID GetNextEntityID();
		static ComponentID GetNextComponentID();

	private:
		static SystemID next_system_id_;
		static EntityID next_entity_id_;
		static ComponentID next_component_id_;

	};

} // End of namespace ~ DECS
