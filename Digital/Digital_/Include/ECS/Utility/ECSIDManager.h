#pragma once

#include <stdint.h>

namespace DECS 
{

	typedef int64_t SystemID;
	typedef int64_t EntityID;
	typedef int64_t ComponentID;

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
