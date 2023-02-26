#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Managers/ECSComponentManager.h>
#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>
#include <Modules/ECS/Objects/ECSEntityRegistry.h>
#include <Modules/ECS/Utility/ECSEntityType.h>

#include <Utility/FamiltyTypeID.h>
#include <Utility/TemplateUtility.h>

#include <string>

namespace DFW
{
	namespace DECS
	{
		// FW Declare.
		class ECSModule;

		template <typename EntityType>
		concept IsValidEntityType = IsBasedOf<EntityType, Entity>;

		class EntityManager final
		{
		private:
			friend ECSModule;

		public:
			EntityManager() = default;
			~EntityManager() = default;

			template <typename EntityType = Entity, typename... TArgs>
			requires IsValidEntityType<EntityType>
			Entity CreateEntity(EntityRegistry& a_registry, TArgs&&... a_args) const;
			
			void DestroyEntity(Entity const& a_entity) const;
			Entity GetEntity(DFW::DUID const a_entity_id, EntityRegistry& a_registry) const;
			
			Entity AttachEntity(Entity const& a_child, Entity const& a_parent) const;
			
			void SetEntityName(Entity const& a_entity, std::string const& a_new_entity_name);
			std::string GetEntityName(Entity const& a_entity) const;

			template <typename EntityType>
			requires IsValidEntityType<EntityType>
			EntityTypeID GetEntityTypeID() const;
			EntityTypeID GetEntityTypeID(Entity const& a_entity) const;

		public:
			// Forwarded Component Mangement Functions.
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
			void ManageDeletedEntities(EntityRegistry& a_registry);

			ComponentManager _component_manager;

		};

#pragma region Template Function Implementation

		template <typename EntityType, typename... TArgs>
		requires IsValidEntityType<EntityType>
		Entity EntityManager::CreateEntity(EntityRegistry& a_registry, TArgs&&... a_args) const
		{
			if (!a_registry.IsValid())
			{
				DFW_ERRORLOG("Attempting to create a new entitiy, but the registry is invalid.");
				DFW_ASSERT(a_registry.IsValid() && "Attempting to create a new entitiy, but the registry is invalid.");
				return EntityType();
			}

			// Construct an Entity from template.
			EntityType entity(std::forward<TArgs>(a_args)...);
			entity._handle		= a_registry.registry.create();
			entity._registry	= &a_registry;
			entity._id			= DFW::GenerateDUID();

			// Register Additional Entity Data in EntityRegistry registries.
			EntityDataComponent& reg_comp = AddComponent<EntityDataComponent>(entity);
			reg_comp.id		= entity._id;
			reg_comp.type	= DUtility::FamilyType<Entity>::GetTypeID<EntityType>();
			reg_comp.name	= DFW_DEFAULT_ENTITY_NAME;

			a_registry.RegisterEntity(entity, reg_comp);
			
			return entity;
		}

		template <typename EntityType>
		requires IsValidEntityType<EntityType>
		EntityTypeID EntityManager::GetEntityTypeID() const
		{
			return DUtility::FamilyType<Entity>::GetTypeID<EntityType>();
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
