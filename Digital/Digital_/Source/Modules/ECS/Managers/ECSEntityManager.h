#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Managers/ECSComponentManager.h>
#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <entt/entity/registry.hpp>

namespace DECS 
{
	// FW Declare.
	class ECSModule;

	class ECSEntityManager final 
	{
	public:
		~ECSEntityManager() = default;

		template <typename EntityType, typename... TArgs>
		EntityType CreateEntity(ECSUniverse* a_universe, TArgs&&... a_args) const;
		ECSEntity CreateEntity(ECSUniverse* a_universe) const;

		void DestroyEntity(ECSEntity const& a_entity) const;
		void DestroyEntity(DCore::DUID a_entity_id, ECSUniverse* a_universe) const;

		ECSEntity GetEntity(DCore::DUID a_entity_id, ECSUniverse* a_universe) const;

		ECSEntity AttachEntity(DCore::DUID a_child_id, DCore::DUID a_parent_id, ECSUniverse* a_universe) const;
		ECSEntity AttachEntity(ECSEntity const& a_child, ECSEntity const& a_parent) const;

		template <typename ComponentType, typename... TArgs>
		ComponentType* const AddComponent(ECSEntity const& a_entity, TArgs&&...a_args) const;

		template <typename ComponentType>
		ComponentType* const GetComponent(ECSEntity const& a_entity) const;

		template <typename... TArgs>
		bool HasComponents(ECSEntity const& a_entity) const;

		template <typename ComponentType>
		bool DeleteComponent(ECSEntity const& a_entity) const;
		 
	protected:
		friend ECSModule;

		ECSEntityManager() = default;

		void ManageDeletedEntities(ECSUniverse* a_universe);

	private:
		ECSComponentManager _component_manager;

	};

#pragma region Template Function Implementation

	template <typename EntityType, typename... TArgs>
	EntityType ECSEntityManager::CreateEntity(ECSUniverse* a_universe, TArgs&&... a_args) const
	{
		if (!a_universe)
		{
			DFW_ERRORLOG("Attempting to create a new entitiy, but the universe is invalid.");
			DFW_ASSERT(a_universe && "Attempting to create a new entitiy, but the universe is invalid.");
			return EntityType();
		}

		DCore::DUID id		= DCore::DUIDGenerator::GenerateID();
		entt::entity handle	= a_universe->_registry.create();

		EntityType entity(std::forward<TArgs>(a_args)...);
		entity._handle		= handle;
		entity._universe	= a_universe;

		// TODO Set Class/Entity Type ID.
		// entity_data_ptr->type_id = TypeID<EntityType>::Get();

		a_universe->_entities.emplace_back(handle);
		a_universe->_entity_data_registration.emplace(handle, AddComponent<ECSEntityRegistrationComponent>(entity, id, "Default Entity Name"));

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
	ComponentType* const ECSEntityManager::AddComponent(ECSEntity const& a_entity, TArgs&&...a_args) const
	{
		return _component_manager.AddComponent<ComponentType>(a_entity, std::forward<TArgs&&>(a_args)...);
	}

	template <typename ComponentType>
	ComponentType* const ECSEntityManager::GetComponent(ECSEntity const& a_entity) const
	{
		return _component_manager.GetComponent<ComponentType>(a_entity);
	}

	template <typename... TArgs>
	bool ECSEntityManager::HasComponents(ECSEntity const& a_entity) const
	{
		return _component_manager.HasComponents<TArgs...>(a_entity);
	}

	template <typename ComponentType>
	bool ECSEntityManager::DeleteComponent(ECSEntity const& a_entity) const
	{
		return _component_manager.DeleteComponent<ComponentType>(a_entity);
	}

#pragma endregion

} // End of namespace ~ DECS
