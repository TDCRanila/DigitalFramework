#pragma once


#include <Modules/ECS/Entity.h>
#include <Modules/ECS/Managers/EntityRegistry.h>
#include <Modules/ECS/Internal/EntityDataComponent.h>
#include <Modules/ECS/Utility/EntityTypeID.h>

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
			
			Entity AttachEntity(Entity const& a_child, Entity const& a_parent) const;
			
			template <typename EntityType>
			requires IsValidEntityType<EntityType>
			EntityTypeID GetEntityTypeID() const;

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
			entity._handle		= a_registry.ENTT().create();
			entity._registry	= &a_registry;

			// Setup additional Entity data.
			EntityDataComponent& reg_comp = entity.AddComponent<EntityDataComponent>();
			reg_comp.id		= DFW::GenerateDUID();
			reg_comp.type	= DUtility::FamilyType<Entity>::GetTypeID<EntityType>();
			reg_comp.name	= DFW_DEFAULT_ENTITY_NAME + std::to_string(static_cast<uint32>(entity._handle));

			// Register Entity in EntityRegistry registers.
			a_registry.RegisterEntity(entity);
			
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
