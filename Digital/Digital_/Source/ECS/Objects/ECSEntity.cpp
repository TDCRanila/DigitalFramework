#include <ECS/Objects/ECSEntity.h>

#include <ECS/Utility/ECSIDManager.h>
#include <ECS/Managers/ECSEntityManager.h>

namespace DECS 
{

	ECSEntity::ECSEntity() :
		entity_id_(UNASSIGNED_ENTITY_ID),
		entity_manager_(nullptr)
	{ 
		/*EMPTY*/ 
	}

	ECSEntity::ECSEntity(EntityID a_entity_id, ECSEntityManager* a_entity_manager) :
		entity_id_(a_entity_id),
		entity_manager_(a_entity_manager)
	{
		/*EMPTY*/
	}

	ECSEntity::~ECSEntity() { /*EMPTY*/ }

	EntityID ECSEntity::GetID() const 
	{
		return entity_id_;
	}

	bool ECSEntity::IsEntityValid() const
	{
		bool is_entity_valid = (entity_id_ == UNASSIGNED_ENTITY_ID || entity_manager_ == nullptr);
		if (is_entity_valid)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool ECSEntity::IsPendingDeletion() const
	{
		return entity_manager_->IsEntityPendingDeletion(entity_id_);
	}

} // End of namespace ~ DECS
