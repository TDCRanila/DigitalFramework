#pragma once


#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>
#include <Modules/ECS/Objects/ECSEntityRegistry.h>
#include <Modules/ECS/Utility/ECSEntityType.h>

#include <CoreSystems/DUID.h>
#include <CoreSystems/Logging/Logger.h>

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

		private:
			void ManageDeletedEntities(EntityRegistry& a_registry);

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

			// Register Additional Entity Data in EntityRegistry registries.
			EntityDataComponent& reg_comp = entity.AddComponent<EntityDataComponent>();
			reg_comp.id		= DFW::GenerateDUID();
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

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
