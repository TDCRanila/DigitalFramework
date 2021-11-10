#include <Modules/ECS/Managers/ECSEntityManager.h>

#include <CoreSystems/Logging/Logger.h>

#include <vector>

namespace ECS 
{
	Entity EntityManager::CreateEntity(Universe* a_universe) const
	{
		return CreateEntity<Entity>(a_universe);
	}

	void EntityManager::DestroyEntity(Entity const& a_entity) const
	{
		if (!a_entity.IsEntityValid())
		{
			DFW_WARNLOG("Attempting to delete an entity that is not valid.");
			return;
		}
		
		std::vector<EntityHandle>& vec = a_entity._universe->_deleted_entities;
		if (auto const it = std::find(vec.begin(), vec.end(), a_entity._handle); it != vec.end())
		{
			DFW_LOG("Attemping to delete an entity that is already marked for deletion.");
			return;
		}

		vec.emplace_back(a_entity._handle);
	}

	Entity EntityManager::GetEntity(DCore::DUID a_entity_id, Universe* a_universe) const
	{
		// TODO : If function becomes used very frequently - might need to rework this function/container. 
		// (Adding a separate container or a bi map; DUID - EntityHandle)
		DFW_ASSERT(a_universe && "Attempting to find an entity in an invalid universe.");

		auto const& result = std::find_if(a_universe->_entity_data_registration.begin(), a_universe->_entity_data_registration.end()
									, [&a_entity_id](auto const& it) { return it.second->id == a_entity_id; });

		if (result != a_universe->_entity_data_registration.end())
			return Entity(result->first, a_universe);
		else
			return Entity();
	}

	void EntityManager::ManageDeletedEntities(Universe* a_universe)
	{
		DFW_ASSERT(a_universe && "Attempting to manage entities, but the universe is invalid.");

		if (a_universe->_deleted_entities.size() == 0)
			return;

		for (EntityHandle const& const handle : a_universe->_deleted_entities)
		{
			// TODO Destroy Entity Event Broadcast.

			a_universe->_entity_data_registration.erase(handle);

			auto const& it = std::find(a_universe->_entities.begin(), a_universe->_entities.end(), handle);
			a_universe->_entities.erase(it);

			a_universe->_registry.destroy(handle);
		}
			
		a_universe->_deleted_entities.clear();
	}

} // End of namespace ~ ECS
