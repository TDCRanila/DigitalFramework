#pragma once

#include <Modules/ECS/Utility/ECSKeyLockSystem.h>
#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

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
		public:
			~ComponentManager() = default;

			template <typename ComponentType>
			ComponentType* const GetComponent(Entity const& a_entity) const;

			template <typename... TArgs>
			bool HasComponents(Entity const& a_entity) const;

			template <typename ComponentType, typename ...TArgs>
			ComponentType* const AddComponent(Entity const& a_entity, TArgs&&...a_args) const;

			template <typename ComponentType>
			bool DeleteComponent(Entity const& a_entity) const;

		protected:
			friend EntityManager;

			ComponentManager();

		private:
			KeyLockSystem _keylock_system;

		};

#pragma region Template Function Implementation

		template <typename ComponentType>
		ComponentType* const ComponentManager::GetComponent(Entity const& a_entity) const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to get a Component of type ComponentType that isn'ComponentType derived from DECS::Component.");
				return nullptr;
			}
			else if constexpr (IsValidComponentType<ComponentType>)
			{
				DFW_ASSERT(a_entity.IsEntityValid() && "Trying to get a component to an invalid entity.");
				ComponentType* component = a_entity._universe->_registry.try_get<ComponentType>(a_entity);
				return component;
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
				bool result = a_entity._universe->_registry.all_of<TArgs...>(a_entity);
				return result;
			}
		}

		template <typename ComponentType, typename... TArgs>
		ComponentType* const ComponentManager::AddComponent(Entity const& a_entity, TArgs&&... a_args) const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to add a Component of type ComponentType that isn'ComponentType derived from DECS::Component.");
				return nullptr;
			}
			else if constexpr (IsValidComponentType<ComponentType>)
			{
				DFW_ASSERT(a_entity.IsEntityValid() && "Trying to add a component to an invalid entity.");
				if (!HasComponents<ComponentType>(a_entity))
				{
					ComponentType& component = a_entity._universe->_registry.emplace<ComponentType>(a_entity, std::forward<TArgs>(a_args)...);
					component._owner = a_entity;
					component._id = DFW::GenerateDUID();

					// Special Case for Entity Registration Component.
					// TODO: Not all that nice, could look in an alternative.
					if constexpr (AreSameTypes<EntityRegistrationComponent, ComponentType>)
					{
						_keylock_system.SetComponentBits<ComponentType>(component.comp_list);
					}
					else
					{
						auto const reg_comp = a_entity._universe->_entity_data_registration[a_entity._handle];
						_keylock_system.SetComponentBits<ComponentType>(reg_comp->comp_list);
					}

					return &component;
				}
			}

			return nullptr;
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
					a_entity._universe->_registry.remove<ComponentType>(a_entity);

					auto const reg_comp = a_entity._universe->_entity_data_registration[a_entity._handle];
					_keylock_system.ResetComponentBits<ComponentType>(reg_comp->comp_list);

					return true;
				}
			}

			return false;
		}

#pragma endregion

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
