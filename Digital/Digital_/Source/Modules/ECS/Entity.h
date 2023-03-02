#pragma once

#include <Modules/ECS/Managers/EntityRegistry.h>
#include <Modules/ECS/Internal/InternalComponent.h>
#include <Modules/ECS/Internal/InternalEntity.h>
#include <Modules/ECS/Utility/ECSTemplateUtility.h>
#include <Modules/ECS/Utility/EntityTypeID.h>

#include <CoreSystems/DUID.h>

#include <Utility/AutoFactory/AutoFactory.h>

#include <string>

namespace DFW
{
	namespace DECS
	{
		// FW Declare.
		class EntityManager;

		class Entity : public InternalEntity, public DFactory::AutoFactory<Entity>
		{
		public:
			Entity() = default;
			Entity(EntityHandle a_entity_handle, EntityRegistry& a_registry);
			~Entity() = default;

			std::strong_ordering operator<=>(Entity const& a_other) const;

		public:
			DFW::DUID GetID() const;
			EntityTypeID GetTypeID() const;

			std::string GetName() const;
			void SetName(std::string const& a_new_name);

			bool IsPendingDeletion() const;

		public:
			// Entity Component Management
			template <typename ComponentType, typename... TArgs>
			ComponentType& AddComponent(TArgs&&...a_args) const;

			template <typename ComponentType>
			bool DeleteComponent() const;

			template <typename ComponentType>
			ComponentType& GetComponent() const;

			template <typename ComponentType>
			ComponentType* const TryGetComponent() const;

			template <typename... TArgs>
			bool HasComponents() const;

		};

#pragma region Template Function Implementation

		template <typename ComponentType, typename... TArgs>
		ComponentType& Entity::AddComponent(TArgs&&...a_args) const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to add a ComponentType that isn't derived from DECS::Component.");
			}
			else
			{
				DFW_ASSERT(IsEntityValid() && "Trying to add a component to an invalid entity.");
				// Check if the component has already been added. Otherwise construct and emplace a new Component of type.
				if (ComponentType* component_ptr = TryGetComponent<ComponentType>())
				{
					return *component_ptr;
				}
				else
				{
					ComponentType& component = _registry->registry.emplace<ComponentType>(_handle, std::forward<TArgs>(a_args)...);
					component._id = DFW::GenerateDUID();
					component._owner = *this;
					return component;
				}
			}
		}

		template <typename ComponentType>
		bool Entity::DeleteComponent() const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to delete a ComponentType that isn't derived from DECS::Component.");
			}
			else
			{
				DFW_ASSERT(IsEntityValid() && "Trying to remove a component from an invalid entity.");
				if (HasComponents<ComponentType>())
				{
					_registry->registry.remove<ComponentType>(_handle);
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		template <typename ComponentType>
		ComponentType& Entity::GetComponent() const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to get a ComponentType that isn't derived from DECS::Component.");
			}
			else
			{
				DFW_ASSERT(IsEntityValid() && "Trying to get a component from an invalid entity.");
				DFW_ASSERT(HasComponents<ComponentType>() && "Trying to get a component that the entity doesn't own.");
				return _registry->registry.get<ComponentType>(_handle);
			}
		}

		template <typename ComponentType>
		ComponentType* const Entity::TryGetComponent() const
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to get a ComponentType that isn't derived from DECS::Component.");
			}
			else if constexpr (IsValidComponentType<ComponentType>)
			{
				DFW_ASSERT(IsEntityValid() && "Trying to get a component from an invalid entity.");
				return _registry->registry.try_get<ComponentType>(_handle);
			}
		}

		template <typename... TArgs>
		bool Entity::HasComponents() const
		{
			if constexpr (sizeof...(TArgs) <= 0)
			{
				static_assert(IsAlwaysFalse<TArgs...>, __FUNCTION__ " - Trying to check component information of entity, but no template arguments provided.");
			}
			else if constexpr ((not IsValidComponentType<TArgs> || ...))
			{
				static_assert(IsAlwaysFalse<TArgs...>, __FUNCTION__ " - Trying to check for a ComponentType that isn't derived from DECS::Component.");
			}
			else
			{
				DFW_ASSERT(IsEntityValid() && "Trying to fetch component information from an invalid entity.");
				return _registry->registry.all_of<TArgs...>(_handle);
			}
		}

#pragma endregion

	} // End of namespace ~ DECS.

	using Entity = DECS::Entity;

} // End of namespace ~ DFW.
