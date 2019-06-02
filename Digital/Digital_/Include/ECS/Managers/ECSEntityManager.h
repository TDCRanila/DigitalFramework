#pragma once

#include <ECS/Utility/ECSIDManager.h>

#include <ECS/Objects/ECSEntityData.h>
#include <ECS/Utility/ECSCompBitList.h>

#include <unordered_map>

namespace DECS 
{
	class ECSEntity;

	class ECSEntityManager final 
	{
	public:
		ECSEntityManager();
		~ECSEntityManager();

		ECSEntity CreateEntity();

		void DeleteEntity(EntityID a_entity_id);

		ECSEntity GetEntity(EntityID a_entity_id);

		bool IsEntityPendingDeletion(EntityID a_entity_id);

		ComponentBitList& GetComponentBitList(EntityID a_entity_id);

	protected:
		friend class ECSModule;

		void Init();

		void ManageEntities();

		void Terminate();

	private:
		void ManageDeletedEntities();

		ECSEntityData* GetEntityData(EntityID a_entity_id);

		std::unordered_map<EntityID, ECSEntityData> entities_;
		std::unordered_map<EntityID, ECSEntityData&> pending_delete_entities_;

		bool manage_deleted_entities_flag_;
		size_t entitymap_reservation_amount_;

	};

} // End of namespace ~ DECS
