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

			auto const& [it, result] = a_entity._registry->_pending_deletion_entities.emplace(a_entity.GetHandle());
			if (!result)
				DFW_WARNLOG("Attemping to delete an entity that is already marked for deletion.");
		}

		Entity EntityManager::GetEntity(DFW::DUID const a_entity_id, EntityRegistry& a_registry) const
		{
			if (!a_registry.IsValid())
			{
				DFW_ERRORLOG("Attempting to find an entitiy, but the registry is invalid.");
				DFW_ASSERT(a_registry.IsValid() && "Attempting to find an entity in an invalid registry.");
				return Entity();
			}

			EntityDUIDRegistrationMap const& umap = a_registry._entity_duid_registration;
			if (auto const& it = umap.find(a_entity_id); it == umap.end())
				return Entity();
			else
				return Entity(it->second, a_registry);
		}

		EntityTypeID EntityManager::GetEntityTypeID(Entity const& a_entity) const
		{
			DFW_ASSERT(a_entity.IsEntityValid());
			return a_entity.GetComponent<EntityDataComponent>().type;
		}

		void EntityManager::SetEntityName(Entity const& a_entity, std::string const& a_new_entity_name)
		{
			DFW_ASSERT(a_entity.IsEntityValid());
			a_entity.GetComponent<EntityDataComponent>().name = a_new_entity_name;
		}
		
		std::string EntityManager::GetEntityName(Entity const& a_entity) const
		{
			DFW_ASSERT(a_entity.IsEntityValid());
			return a_entity.GetComponent<EntityDataComponent>().name;
		}

		void EntityManager::ManageDeletedEntities(EntityRegistry& a_registry)
		{
			DFW_ASSERT(a_registry.IsValid() && "Attempting to manage entities, but the registry is invalid.");

			if (a_registry._pending_deletion_entities.empty())
				return;

			for (EntityHandle const handle : a_registry._pending_deletion_entities)
			{
				a_registry.UnregisterEntity(Entity(handle, a_registry));
				a_registry.registry.destroy(handle);
			}

			a_registry._pending_deletion_entities.clear();
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
