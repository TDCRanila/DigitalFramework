#pragma once

#include <Modules/ECS/Utility/ECSKeyLockSystem.h>
#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Objects/ECSEntityRegistry.h>

#include <Utility/TemplateUtility.h>
#include <CoreSystems/DUID.h>

namespace DFW
{
	namespace DECS
	{
		// FW Declare
		class EntityManager;

		class ComponentManager final
		{
		private:
			friend EntityManager;

		public:
			ComponentManager();
			~ComponentManager() = default;

			template <typename ComponentType>
			ComponentType& GetComponent(Entity const& a_entity) const;

			template <typename ComponentType>
			ComponentType* const TryGetComponent(Entity const& a_entity) const;

			template <typename... TArgs>
			bool HasComponents(Entity const& a_entity) const;

			template <typename ComponentType, typename ...TArgs>
			ComponentType& AddComponent(Entity const& a_entity, TArgs&&...a_args) const;

			template <typename ComponentType>
			bool DeleteComponent(Entity const& a_entity) const;

		private:
			KeyLockSystem _keylock_system;

		};

#pragma region Template Function Implementation

		template <typename ComponentType>
		ComponentType& ComponentManager::GetComponent(Entity const& a_entity) const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to get a Component of type ComponentType that isn'ComponentType derived from DECS::Component.");
			}
			else if constexpr (IsValidComponentType<ComponentType>)
			{
				DFW_ASSERT(a_entity.IsEntityValid() && "Trying to get a component of an invalid entity.");
				DFW_ASSERT(HasComponents<ComponentType>(a_entity) && "Trying to get a component that the entity doesn't own.");
				return a_entity._registry->registry.get<ComponentType>(a_entity);
			}
		}

		template <typename ComponentType>
		ComponentType* const ComponentManager::TryGetComponent(Entity const& a_entity) const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to get a Component of type ComponentType that isn'ComponentType derived from DECS::Component.");
				return nullptr;
			}
			else if constexpr (IsValidComponentType<ComponentType>)
			{
				DFW_ASSERT(a_entity.IsEntityValid() && "Trying to get a component of an invalid entity.");
				return a_entity._registry->registry.try_get<ComponentType>(a_entity);
			}
		}

		template <typename... TArgs>
		bool ComponentManager::HasComponents(Entity const& a_entity) const
		{
			if constexpr (sizeof...(TArgs) <= 0)
			{
				static_assert(IsAlwaysFalse<TArgs...>, __FUNCTION__ " - Trying to check the presence of a Component, but no template arguments provided.");
				return false;
			}
			else if constexpr ((not IsValidComponentType<TArgs> || ...))
			{
				static_assert(IsAlwaysFalse<TArgs...>, __FUNCTION__ " - Trying to check for a Component of type ComponentType that isn'ComponentType derived from DECS::Component.");
				return false;
			}
			else
			{
				DFW_ASSERT(a_entity.IsEntityValid() && "Trying to read component data from an invalid entity.");
				return a_entity._registry->registry.all_of<TArgs...>(a_entity);
			}
		}

		template <typename ComponentType, typename... TArgs>
		ComponentType& ComponentManager::AddComponent(Entity const& a_entity, TArgs&&... a_args) const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to add a Component of type ComponentType that isn't derived from DECS::Component.");
			}
			else if constexpr (IsValidComponentType<ComponentType>)
			{
				DFW_ASSERT(a_entity.IsEntityValid() && "Trying to add a component to an invalid entity.");
				if (HasComponents<ComponentType>(a_entity))
				{
					return GetComponent<ComponentType>(a_entity);
				}
				else
				{
					// Construct and emplace the Component.
					EntityRegistry& registry(*a_entity._registry);
					ComponentType& component	= registry.registry.emplace<ComponentType>(a_entity, std::forward<TArgs>(a_args)...);
					component._owner			= a_entity;
					component._id				= DFW::GenerateDUID();

					// Update ComponentBitList of the entity
					EntityDataComponent* reg_comp;
					if constexpr (AreSameTypes<EntityDataComponent, ComponentType>)
						reg_comp = &component;
					else
						reg_comp = &registry._entity_handle_registration.at(a_entity).get();

					_keylock_system.SetComponentBits<ComponentType>(reg_comp->comp_list);

					return component;
				}
			}
		}

		template <typename ComponentType>
		bool ComponentManager::DeleteComponent(Entity const& a_entity) const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to delete a Component of type ComponentType that isn'ComponentType derived from DECS::Component.");
				return false;
			}
			else if constexpr (IsValidComponentType<ComponentType>)
			{
				DFW_ASSERT(a_entity.IsEntityValid() && "Trying to remove a component from an invalid entity.");
				if (HasComponents<ComponentType>(a_entity))
				{
					EntityRegistry& registry(*a_entity._registry);
					registry.registry.remove<ComponentType>(a_entity);

					EntityDataComponent& reg_comp = registry._entity_handle_registration.at(a_entity).get();
					_keylock_system.ResetComponentBits<ComponentType>(reg_comp.comp_list);

					return true;
				}
				else
				{
					return false;
				}
			}
		}

#pragma endregion

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
