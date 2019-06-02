#include <ECS/Objects/ECSEntityData.h>

namespace DECS
{
	ECSEntityData::ECSEntityData() :
		entity_id(0),
		component_bit_list(0),
		pending_delete(false)
	{
		/*EMPTY*/
	}

	ECSEntityData::ECSEntityData(EntityID a_entity_id) :
		entity_id(a_entity_id),
		component_bit_list(0),
		pending_delete(false)
	{
		/*EMPTY*/
	}

	ECSEntityData::~ECSEntityData()
	{
		/*EMPTY*/
	}

} // End of namespace ~ DECS
