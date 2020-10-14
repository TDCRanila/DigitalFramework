#include <Modules/ECS/Managers/ECSEntityManager.h>

#include <Modules/ECS/Objects/ECSEntity.h>

namespace DECS 
{

	ECSEntityManager::ECSEntityManager() :
		manage_deleted_entities_flag_(false),
		entitymap_reservation_amount_(1000)
	{ /*EMPTY*/ }
	
	ECSEntityManager::~ECSEntityManager() { /*EMPTY*/ }

	ECSEntity ECSEntityManager::CreateEntity()
	{
		const EntityID new_id(ECSIDManager::GetNextEntityID());
		entities_.emplace(new_id, ECSEntityData(new_id));	
		return ECSEntity (new_id, this);;
	}

	void ECSEntityManager::DeleteEntity(EntityID a_entity_id)
	{
		ECSEntityData* entity_data			= GetEntityData(a_entity_id);
		const ECSEntity entity_to_delete	= GetEntity(a_entity_id);
		if (entity_to_delete.IsEntityValid() && !entity_to_delete.IsPendingDeletion())
		{
			entity_data->pending_delete = true;
			pending_delete_entities_.emplace(a_entity_id, (*entity_data));
		}
		else
		{
			return;
		}
	}

	ECSEntity ECSEntityManager::GetEntity(EntityID a_entity_id)
	{
		const auto& it = this->entities_.find(a_entity_id);
		if (it == this->entities_.end())
		{
			DFW_INFOLOG("Trying to query Entity with ID: " << a_entity_id << " - but cannot be found.");
			return ECSEntity();
		}
		else
		{
			return ECSEntity(a_entity_id, this);
		}
	}

	ECSEntityData* ECSEntityManager::GetEntityData(EntityID a_entity_id)
	{
		const auto& it = this->entities_.find(a_entity_id);
		if (it == this->entities_.end())
		{
			DFW_INFOLOG("Trying to query Entity with ID: " << a_entity_id << " - but cannot be found.");
			return nullptr;
		}
		else
		{
			return &it->second;
		}
	}

	bool ECSEntityManager::IsEntityPendingDeletion(EntityID a_entity_id)
	{
		// TODO Safety Assert or Resolve
		return GetEntityData(a_entity_id)->pending_delete;
	}

	ComponentBitList& ECSEntityManager::GetComponentBitList(EntityID a_entity_id)
	{
		// TODO Safety Assert or Resolve
		return GetEntityData(a_entity_id)->component_bit_list;
	}

	void ECSEntityManager::Init()
	{
		entities_.reserve(entitymap_reservation_amount_);
		pending_delete_entities_.reserve(entitymap_reservation_amount_);
	}

	void ECSEntityManager::Terminate()
	{
		entities_.clear();
		pending_delete_entities_.clear();
	}

	void ECSEntityManager::ManageEntities()
	{
		if (manage_deleted_entities_flag_)
		{
			if (pending_delete_entities_.empty())
			{
				DFW_ERRORLOG("Dirty Flag enabled the management of the entities, but there are none to manage.");
				// TODO. Remove? Needs Testing.
				DFW_ASSERT(false);
			}
			ManageDeletedEntities();
			manage_deleted_entities_flag_ = false;
		}
	}

	void ECSEntityManager::ManageDeletedEntities()
	{
		// TODO.
	}

} // End of namespace ~ DECS
