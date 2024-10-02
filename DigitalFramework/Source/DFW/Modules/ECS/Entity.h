#pragma once

#include <DFW/Modules/ECS/Managers/EntityRegistry.h>
#include <DFW/Modules/ECS/Internal/InternalComponent.h>
#include <DFW/Modules/ECS/Internal/InternalEntity.h>
#include <DFW/Modules/ECS/Utility/ECSTemplateUtility.h>
#include <DFW/Modules/ECS/Utility/EntityTypeID.h>

namespace DFW
{
	namespace DECS
	{
		class Entity : public InternalEntity
		{
		public:
			Entity() = default;
			Entity(EntityHandle a_entity_handle, EntityRegistry& a_registry);
			~Entity() = default;

			bool operator==(Entity const& a_other) const;

		public:
			DFW::DUID GetID() const;
			EntityTypeID GetTypeID() const;

			std::string const& GetName() const;
			void SetName(std::string const& a_new_name);
			
			EntityTypeID GetType() const;
			template <StringLiteral entity_type>
			void SetType();

		public:
			void AddChild(Entity a_child_to_add);
			void RemoveChild(Entity a_child_to_remove);
			void SetParent(Entity a_parent);

			bool IsParentOfEntity(Entity const& a_child) const;
			bool IsChildOfEntity(Entity const& a_parent) const;
			bool IsChildInEntityHierachyOf(Entity const& a_parent) const;

		public:
			// Entity Component Management.
			template <typename ComponentType, typename... TArgs>
			ComponentType& AddComponent(TArgs&&...a_args);

			template <typename ComponentType>
			bool DeleteComponent();

			template <typename ComponentType>
			ComponentType& GetComponent();

			template <typename ComponentType>
			ComponentType const& GetComponent() const;

			template <typename ComponentType>
			ComponentType* const TryGetComponent();

			template <typename ComponentType>
			ComponentType const* const TryGetComponent() const;

			template <typename... TArgs>
			bool HasComponents() const;

		private:
			void SetTypeInternal(EntityTypeID const a_entity_type_id);

		};

#pragma region Template Function Implementation

		template <StringLiteral entity_type>
		void Entity::SetType()
		{
			DFW_ASSERT(IsEntityValid() && "Trying to change the type of an invalid entity.");
			SetTypeInternal(_registry->GetEntityTypeID<entity_type>());
		}

		template <typename ComponentType, typename... TArgs>
		ComponentType& Entity::AddComponent(TArgs&&...a_args)
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
					ComponentType& component = _registry->ENTT().emplace<ComponentType>(_handle, std::forward<TArgs>(a_args)...);
					component._id = DFW::GenerateDUID();
					component._owner = *this;
					return component;
				}
			}
		}

		template <typename ComponentType>
		bool Entity::DeleteComponent()
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
					_registry->ENTT().remove<ComponentType>(_handle);
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		template <typename ComponentType>
		ComponentType& Entity::GetComponent()
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to get a ComponentType that isn't derived from DECS::Component.");
			}
			else
			{
				DFW_ASSERT(IsEntityValid() && "Trying to get a component from an invalid entity.");
				DFW_ASSERT(HasComponents<ComponentType>() && "Trying to get a component that the entity doesn't own.");
				return _registry->ENTT().get<ComponentType>(_handle);
			}
		}

		template <typename ComponentType>
		ComponentType const& Entity::GetComponent() const
		{
			return const_cast<Entity*>(this)->GetComponent<ComponentType>();
		}

		template <typename ComponentType>
		ComponentType* const Entity::TryGetComponent()
		{
			if constexpr (not IsValidComponentType<ComponentType>)
			{
				static_assert(always_false<ComponentType>::value, __FUNCTION__ " - Trying to get a ComponentType that isn't derived from DECS::Component.");
			}
			else if constexpr (IsValidComponentType<ComponentType>)
			{
				DFW_ASSERT(IsEntityValid() && "Trying to get a component from an invalid entity.");
				return _registry->ENTT().try_get<ComponentType>(_handle);
			}
		}

		template <typename ComponentType>
		ComponentType const* const Entity::TryGetComponent() const
		{
			return const_cast<Entity*>(this)->TryGetComponent<ComponentType>();
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
				return _registry->ENTT().all_of<TArgs...>(_handle);
			}
		}

#pragma endregion

	} // End of namespace ~ DECS.

	using Entity = DECS::Entity;

} // End of namespace ~ DFW.
