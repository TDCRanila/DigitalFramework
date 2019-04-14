#pragma once

#include <ECS/Utility/ECSIDManager.h>

#include <vector>

namespace DECS 
{

	class ECSEntityManager final 
	{
	public:
		ECSEntityManager();
		~ECSEntityManager();

		EntityID CreateEntity();
		void DeleteEntity(EntityID a_entity_id);

	private:
		void ManageDeletedEntities();

		std::vector<EntityID> entities_;
		std::vector<EntityID> pending_delete_entities;

	};

} // End of namespace ~ DECS
