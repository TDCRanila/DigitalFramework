#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Managers/ECSComponentManager.h>
#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <entt/entity/registry.hpp>

namespace DFW
{
	namespace DECS
	{
		// FW Declare.
		class ECSModule;

		class EntityManager final
		{
		private:
			friend ECSModule;

		public:
			EntityManager() = default;
			~EntityManager() = default;

			template <typename EntityType, typename... TArgs>
			EntityType CreateEntity(Universe& a_universe, TArgs&&... a_args) const;
			Entity CreateEntity(Universe& a_universe) const;

			void DestroyEntity(Entity const& a_entity) const;
			void DestroyEntity(DFW::DUID const a_entity_id, Universe& a_universe) const;

			Entity GetEntity(DFW::DUID const a_entity_id, Universe& a_universe) const;

			Entity AttachEntity(Entity const& a_child, Entity const& a_parent) const;

			template <typename ComponentType, typename... TArgs>
			ComponentType& AddComponent(Entity const& a_entity, TArgs&&...a_args) const;

			template <typename ComponentType>
			ComponentType& GetComponent(Entity const& a_entity) const;

			template <typename ComponentType>
			ComponentType* const TryGetComponent(Entity const& a_entity) const;

			template <typename... TArgs>
			bool HasComponents(Entity const& a_entity) const;

			template <typename ComponentType>
			bool DeleteComponent(Entity const& a_entity) const;

		private:
			void ManageDeletedEntities(Universe& a_universe);

		private:
			ComponentManager _component_manager;

		};

#pragma region Template Function Implementation

		template <typename EntityType, typename... TArgs>
		EntityType EntityManager::CreateEntity(Universe& a_universe, TArgs&&... a_args) const
		{
			if (!a_universe.IsValid())
			{
				DFW_ERRORLOG("Attempting to create a new entitiy, but the universe is invalid.");
				DFW_ASSERT(a_universe.IsValid() && "Attempting to create a new entitiy, but the universe is invalid.");
				return EntityType();
			}

			// Request a new entity handle from the registry.
			EntityHandle new_handle = a_universe.registry.create();

			// TODO - Should be in something like a WorldManager
			// Construct an Entity from template.
			EntityType entity(std::forward<TArgs>(a_args)...);
			entity._handle		= new_handle;
			entity._universe	= &a_universe;
			entity._id			= DFW::GenerateDUID();

			// Store entity information in universe containers.
			a_universe._entities.emplace(new_handle);

			a_universe._entity_data_registration.emplace(
				new_handle, 
				AddComponent<EntityRegistrationComponent>(entity, entity._id, DFW_DEFAULT_ENTITY_NAME)
			);

			// TODO Set Class/Entity Type ID.
			// entity_data_ptr->type_id = TypeID<EntityType>::Get();
			
			// TODO - Should be in something like a WorldManager -  Add any additional default components here; e.g. transform, name component
			// _component_manager.AddComponent<TransformComponent>(entity, a_universe);

			// TODO - Should be in something like a WorldManager -  
			// GameObject Custom Arguments.
			// Add EntityCreate Event Broadcast
			/*if constexpr (IsDerivedFrom<EntityType, DFW::GameObject>)
			{
				entity_handle.OnCreate();
			}*/

			return entity;
		}

		template <typename ComponentType, typename... TArgs>
		ComponentType& EntityManager::AddComponent(Entity const& a_entity, TArgs&&...a_args) const
		{
			return _component_manager.AddComponent<ComponentType>(a_entity, std::forward<TArgs&&>(a_args)...);
		}

		template <typename ComponentType>
		ComponentType& EntityManager::GetComponent(Entity const& a_entity) const
		{
			return _component_manager.GetComponent<ComponentType>(a_entity);
		}

		template <typename ComponentType>
		ComponentType* const EntityManager::TryGetComponent(Entity const& a_entity) const
		{
			return _component_manager.TryGetComponent<ComponentType>(a_entity);
		}

		template <typename... TArgs>
		bool EntityManager::HasComponents(Entity const& a_entity) const
		{
			return _component_manager.HasComponents<TArgs...>(a_entity);
		}

		template <typename ComponentType>
		bool EntityManager::DeleteComponent(Entity const& a_entity) const
		{
			return _component_manager.DeleteComponent<ComponentType>(a_entity);
		}

#pragma endregion

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
