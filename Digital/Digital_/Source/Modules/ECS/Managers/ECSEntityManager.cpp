#include <Modules/ECS/Managers/ECSEntityManager.h>

#include <CoreSystems/Logging/Logger.h>

#include <vector>

namespace DFW
{
	namespace DECS
	{
		Entity EntityManager::CreateEntity(Universe& a_universe) const
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

			auto const& [it, result] = a_entity._universe->_pending_deletion_entities.emplace(a_entity);
			if (!result)
				DFW_WARNLOG("Attemping to delete an entity that is already marked for deletion.");
		}

		Entity EntityManager::GetEntity(DFW::DUID a_entity_id, Universe& a_universe) const
		{
			// TODO : If function becomes used very frequently - might need to rework this function/container. 
			// (Adding a separate container or a bi map; DUID - EntityHandle)
			DFW_ASSERT(a_universe.IsValid() && "Attempting to find an entity in an invalid universe.");

			auto search_func = [&a_entity_id](auto const& it) -> bool { return it.second.get().id == a_entity_id; };
			auto const& result = std::find_if(
					a_universe._entity_data_registration.begin()
				,	a_universe._entity_data_registration.end()
				,	search_func);

			if (result != a_universe._entity_data_registration.end())
				return Entity(result->first, a_universe);

			return Entity();
		}

		void EntityManager::ManageDeletedEntities(Universe& a_universe)
		{
			DFW_ASSERT(a_universe.IsValid() && "Attempting to manage entities, but the universe is invalid.");

			if (a_universe._pending_deletion_entities.empty())
				return;

			for (EntityHandle const handle : a_universe._pending_deletion_entities)
			{
				// TODO Destroy Entity Event Broadcast.

				a_universe._entity_data_registration.erase(handle);				
				a_universe._entities.erase(handle);
				a_universe.registry.destroy(handle);
			}

			a_universe._pending_deletion_entities.clear();
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
