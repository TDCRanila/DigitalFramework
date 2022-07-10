#include <Modules/ECS/Managers/ECSEntityManager.h>

#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
	namespace DECS
	{
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

		Entity EntityManager::GetEntity(DFW::DUID const a_entity_id, Universe& a_universe) const
		{
			if (!a_universe.IsValid())
			{
				DFW_ERRORLOG("Attempting to find an entitiy, but the universe is invalid.");
				DFW_ASSERT(a_universe.IsValid() && "Attempting to find an entity in an invalid universe.");
				return Entity();
			}

			EntityDUIDRegistrationMap const& umap = a_universe._entity_duid_registration;
			if (auto const& it = umap.find(a_entity_id); it == umap.end())
				return Entity();
			else
				return Entity(it->second, a_universe);
		}

		EntityTypeID EntityManager::GetEntityTypeID(Entity const& a_entity) const
		{
			DFW_ASSERT(a_entity.IsEntityValid());
			return GetComponent<EntityDataComponent>(a_entity).type;
		}

		void EntityManager::SetEntityName(Entity const& a_entity, std::string const& a_new_entity_name)
		{
			DFW_ASSERT(a_entity.IsEntityValid());
			GetComponent<EntityDataComponent>(a_entity).name = a_new_entity_name;
		}
		
		std::string EntityManager::GetEntityName(Entity const& a_entity) const
		{
			DFW_ASSERT(a_entity.IsEntityValid());
			return GetComponent<EntityDataComponent>(a_entity).name;			
		}

		void EntityManager::ManageDeletedEntities(Universe& a_universe)
		{
			DFW_ASSERT(a_universe.IsValid() && "Attempting to manage entities, but the universe is invalid.");

			if (a_universe._pending_deletion_entities.empty())
				return;

			for (EntityHandle const handle : a_universe._pending_deletion_entities)
			{
				a_universe.UnregisterEntity(Entity(handle, a_universe));
				a_universe.registry.destroy(handle);
			}

			a_universe._pending_deletion_entities.clear();
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
