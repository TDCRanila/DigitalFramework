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
		constexpr StringLiteral DFW_BASE_ENTITY_FAMILY_TYPE_NAME = "Entity";

		class EntityManager final
		{
		private:
			friend ECSModule;

		public:
			EntityManager() = default;
			~EntityManager() = default;

			template <StringLiteral entity_type_name>
			Entity CreateEntity(EntityRegistry& a_registry) const;
			Entity CreateEntity(EntityRegistry& a_registry) const;

			void DestroyEntity(Entity const& a_entity) const;
			
			Entity AttachEntity(Entity const& a_child, Entity const& a_parent) const;
			
			template <StringLiteral entity_type_name>
			EntityTypeID GetEntityTypeID() const;

		private:
			void CleanDestructionMarkedEntities(EntityRegistry& a_registry);

		};

#pragma region Template Function Implementation

		template <StringLiteral entity_type_name>
		Entity EntityManager::CreateEntity(EntityRegistry& a_registry) const
		{
			if (!a_registry.IsValid())
			{
				DFW_ERRORLOG("Attempting to create a new entitiy, but the registry is invalid.");
				DFW_ASSERT(a_registry.IsValid() && "Attempting to create a new entitiy, but the registry is invalid.");
				return Entity();
			}

			// Construct a new Entity.
			Entity entity(a_registry.ENTT().create(), a_registry);

			// Setup additional Entity data.
			EntityDataComponent& data_component = entity.AddComponent<EntityDataComponent>();
			data_component.id = DFW::GenerateDUID();
			data_component.type = GetEntityTypeID<entity_type_name>();
			data_component.name = DFW_DEFAULT_ENTITY_NAME + std::to_string(static_cast<uint32>(entity._handle));

			// Register Entity in EntityRegistry registers.
			a_registry.RegisterEntity(entity);

			return entity;
		}

		template <StringLiteral entity_type_name>
		EntityTypeID EntityManager::GetEntityTypeID() const
		{
			return DUtility::FamilyNameType<DFW_BASE_ENTITY_FAMILY_TYPE_NAME>::GetTypeID<entity_type_name>();
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
