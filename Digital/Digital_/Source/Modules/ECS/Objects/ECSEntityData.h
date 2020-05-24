#pragma once

#include <Modules/ECS/Utility/ECSCompBitList.h>
#include <Modules/ECS/Utility/ECSIDManager.h>

namespace DECS
{
	struct ECSEntityData
	{
	public:
		ECSEntityData();
		~ECSEntityData();

	protected:
		friend class ECSEntityManager;

		ECSEntityData(EntityID a_entity_id);
		
		EntityID			entity_id;
		ComponentBitList	component_bit_list;
		bool				pending_delete;
		
	};

} // End of namespace ~ DECS
