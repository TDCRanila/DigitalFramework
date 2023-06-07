#include <Modules/ECS/Managers/EntityManager.h>

#include <Modules/ECS/Internal/EntityRelationComponent.h>

#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
	namespace DECS
	{
		Entity EntityManager::CreateEntity(EntityRegistry& a_registry) const
		{
			return CreateEntity<DFW_BASE_ENTITY_FAMILY_TYPE_NAME>(a_registry);
		}

		void EntityManager::DestroyEntity(Entity const& a_entity) const
		{
			if (!a_entity.IsEntityValid())
			{
				DFW_WARNLOG("Attempting to delete an entity that is invalid.");
				return;
			}

			DestroyEntityAndChilderen(a_entity);
		}

		void EntityManager::DestroyEntityAndChilderen(Entity const& a_current_entity) const
		{
			std::unordered_set<EntityHandle>& marked_entities = a_current_entity._registry->_marked_entities_for_destruction;
			auto const& [it, insertion_result] = marked_entities.emplace(a_current_entity.GetHandle());
			if (!insertion_result)
				DFW_WARNLOG("Attemping to delete an entity that is already marked for deletion.");

			DECS::EntityRelationComponent const* relation_component = a_current_entity.TryGetComponent<DECS::EntityRelationComponent>();
			if (!relation_component)
				return; // No childeren or siblings.

			// Traverse entity hierachy.
			Entity current_child = relation_component->first;
			while (current_child.IsEntityValid())
			{
				DestroyEntityAndChilderen(current_child);

				if (DECS::EntityRelationComponent const* child_relation_component = current_child.TryGetComponent<DECS::EntityRelationComponent>())
					current_child = child_relation_component->next;
			}
		}

		void EntityManager::CleanDestructionMarkedEntities(EntityRegistry& a_registry)
		{
			DFW_ASSERT(a_registry.IsValid() && "Attempting to manage entities, but the registry is invalid.");

			std::unordered_set<EntityHandle>& marked_entities = a_registry._marked_entities_for_destruction;

			if (marked_entities.empty())
				return;

			for (EntityHandle const handle : marked_entities)
			{
				a_registry.UnregisterEntity(InternalEntity(handle, &a_registry));
			}

			a_registry.ENTT().destroy(marked_entities.begin(), marked_entities.end());

			marked_entities.clear();
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
