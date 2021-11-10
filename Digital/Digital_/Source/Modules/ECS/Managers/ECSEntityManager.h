#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Managers/ECSComponentManager.h>
#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <entt/entity/registry.hpp>

namespace ECS 
{
	// FW Declare.
	class ECSModule;

	class EntityManager final 
	{
	public:
		~EntityManager() = default;

		template <typename EntityType, typename... TArgs>
		EntityType CreateEntity(Universe* a_universe, TArgs&&... a_args) const;
		Entity CreateEntity(Universe* a_universe) const;

		void DestroyEntity(Entity const& a_entity) const;
		void DestroyEntity(DCore::DUID a_entity_id, Universe* a_universe) const;

		Entity GetEntity(DCore::DUID a_entity_id, Universe* a_universe) const;

		Entity AttachEntity(DCore::DUID a_child_id, DCore::DUID a_parent_id, Universe* a_universe) const;
		Entity AttachEntity(Entity const& a_child, Entity const& a_parent) const;

		template <typename ComponentType, typename... TArgs>
		ComponentType* const AddComponent(Entity const& a_entity, TArgs&&...a_args) const;

		template <typename ComponentType>
		ComponentType* const GetComponent(Entity const& a_entity) const;

		template <typename... TArgs>
		bool HasComponents(Entity const& a_entity) const;

		template <typename ComponentType>
		bool DeleteComponent(Entity const& a_entity) const;
		 
	protected:
		friend ECSModule;

		EntityManager() = default;

		void ManageDeletedEntities(Universe* a_universe);

	private:
		ComponentManager _component_manager;

	};

#pragma region Template Function Implementation

	template <typename EntityType, typename... TArgs>
	EntityType EntityManager::CreateEntity(Universe* a_universe, TArgs&&... a_args) const
	{
		if (!a_universe)
		{
			DFW_ERRORLOG("Attempting to create a new entitiy, but the universe is invalid.");
			DFW_ASSERT(a_universe && "Attempting to create a new entitiy, but the universe is invalid.");
			return EntityType();
		}

		entt::entity handle	= a_universe->_registry.create();

		EntityType entity(std::forward<TArgs>(a_args)...);
		entity._handle		= handle;
		entity._universe	= a_universe;

		// TODO Set Class/Entity Type ID.
		// entity_data_ptr->type_id = TypeID<EntityType>::Get();

		a_universe->_entities.emplace_back(handle);
		a_universe->_entity_data_registration.emplace(handle, 
			AddComponent<EntityRegistrationComponent>(entity, DCore::GenerateDUID(), "Default Entity Name"));

		// TODO - Should be in something like a WorldManager -  Add any additional default components here; e.g. transform, name component
		// _component_manager.AddComponent<TransformComponent>(entity, a_universe);

		// TODO - Should be in something like a WorldManager -  
		// GameObject Custom Arguments.
		// Add EntityCreate Event Broadcast
		/*if constexpr (IsDerivedFrom<EntityType, DCore::GameObject>)
		{
			entity_handle.OnCreate();
		}*/

		return entity;
	}

	template <typename ComponentType, typename... TArgs>
	ComponentType* const EntityManager::AddComponent(Entity const& a_entity, TArgs&&...a_args) const
	{
		return _component_manager.AddComponent<ComponentType>(a_entity, std::forward<TArgs&&>(a_args)...);
	}

	template <typename ComponentType>
	ComponentType* const EntityManager::GetComponent(Entity const& a_entity) const
	{
		return _component_manager.GetComponent<ComponentType>(a_entity);
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

} // End of namespace ~ ECS
