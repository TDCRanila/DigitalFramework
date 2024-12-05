#include <DFW/Modules/ECS/Entity.h>

#include <DFW/Modules/ECS/Internal/EntityDataComponent.h>
#include <DFW/Modules/ECS/Internal/EntityRelationComponent.h>

namespace DFW
{
	namespace DECS
	{
		namespace Detail
		{
			bool CheckForEntityInHierachy(Entity const& a_current_entity, Entity const& a_entity_to_check)
			{
				if (a_current_entity == a_entity_to_check)
					return true;

				DECS::EntityRelationComponent const* relation_component = a_current_entity.TryGetComponent<DECS::EntityRelationComponent>();
				if (!relation_component)
					return false; // No childeren or siblings.

				// Traverse entity hierachy.
				Entity const* current_child = &relation_component->first;
				while (current_child->IsEntityValid())
				{
					if (CheckForEntityInHierachy(*current_child, a_entity_to_check))
						return true; // Found entity, no further traversal needed.
					
					current_child = &current_child->GetComponent<DECS::EntityRelationComponent>().next;
				}

				return false;
			}

			void UpdateEntityHierarchyDepthValuesFromEntity(Entity& a_current_entity, uint32 const a_parent_hierarchy_depth, bool const a_is_first_traversal)
			{
				DECS::EntityRelationComponent& relation_component = a_current_entity.GetComponent<DECS::EntityRelationComponent>();
				if (!a_is_first_traversal)
					relation_component.hierarchy_depth = a_parent_hierarchy_depth + 1;

				// Traverse entity hierachy.
				Entity* current_child = &relation_component.first;
				while (current_child->IsEntityValid())
				{
					UpdateEntityHierarchyDepthValuesFromEntity(*current_child, relation_component.hierarchy_depth, false);
					current_child = &current_child->GetComponent<DECS::EntityRelationComponent>().next;
				}
			}

		} // End of namespace ~ Detail.

		Entity::Entity(EntityHandle a_entity_handle, EntityRegistry& a_registry)
			: InternalEntity(a_entity_handle, &a_registry)
		{
		}

		bool Entity::operator==(Entity const& a_other) const
		{
			if (this->_handle != a_other._handle)
				return false;

			if (this->_registry != a_other._registry)
				return false;

			return true;
		}

		DFW::DUID Entity::GetID() const
		{
			return GetComponent<EntityDataComponent>().id;
		}

		std::string const& Entity::GetName() const
		{
			return GetComponent<EntityDataComponent>().name;
		}

		void Entity::SetName(std::string const& a_new_name)
		{
			EntityDataComponent& data_component = GetComponent<EntityDataComponent>();
			std::string& entity_name_ref = data_component.name;

			_registry->_entity_name_register.erase(entity_name_ref);

			entity_name_ref = a_new_name;

			_registry->_entity_name_register.emplace(entity_name_ref, _handle);
		}

		EntityTypeID Entity::GetType() const
		{
			return GetComponent<EntityDataComponent>().type;
		}

		void Entity::SetTypeInternal(EntityTypeID const a_entity_type_id)
		{
			EntityDataComponent& data_component = GetComponent<EntityDataComponent>();
			data_component.type = a_entity_type_id;
		}

		void Entity::AddChild(Entity a_child_to_add)
		{
			if (!a_child_to_add.IsEntityValid())
				return;

			if (this->GetRegistry() != a_child_to_add.GetRegistry())
				return;

			if (IsParentOfEntity(a_child_to_add))
				return;

			// Check if child entity was already owned by another entity,
			// inform the older parent to remove the child.
			EntityRelationComponent& child_relation_component = a_child_to_add.AddComponent<EntityRelationComponent>();
			if (child_relation_component.parent.IsEntityValid())
				child_relation_component.parent.RemoveChild(a_child_to_add);

			// Add child to this entity.
			EntityRelationComponent& relation_component = AddComponent<EntityRelationComponent>();
			// Emplace the child in front of childeren stack.
			if (!relation_component.first.IsEntityValid())
			{
				relation_component.first = a_child_to_add;
			}
			else
			{
				// Set the older child.previous to the new child as its new sibling.
				EntityRelationComponent& first_child_relation_component = relation_component.first.GetComponent<EntityRelationComponent>();
				first_child_relation_component.previous = a_child_to_add;

				// New child should point to the older child.
				child_relation_component.next = relation_component.first;

				// Set this (the Parent) its first child to the new child.
				relation_component.first = a_child_to_add;
			}

			// Now set the parent of the child to this entity;
			child_relation_component.parent = *this;

			// Update hierarchy values.
			Detail::UpdateEntityHierarchyDepthValuesFromEntity(*this, relation_component.hierarchy_depth, true);

			// Update parent's children count.
			relation_component.childeren_count++;
		}

		void Entity::RemoveChild(Entity a_child_to_remove)
		{
			if (!a_child_to_remove.IsEntityValid())
				return;

			if (this->GetRegistry() != a_child_to_remove.GetRegistry())
				return;

			if (!IsParentOfEntity(a_child_to_remove))
				return; // Cannot remove, when not the parent of this child.

			// Attempt to remove child.
			EntityRelationComponent& relation_component = GetComponent<EntityRelationComponent>();
			EntityRelationComponent& child_relation_component = a_child_to_remove.GetComponent<EntityRelationComponent>();

			// If the child that is being removed is the first child of the parent,
			// shift the entity first to the next or empty sibling of the child.
			if (relation_component.first == a_child_to_remove)
			{
				if (relation_component.childeren_count - 1 == 0)
					relation_component.first = Entity();
				else
					relation_component.first = child_relation_component.next;
			}

			// Adjust sibling relationships if there are any.
			if (relation_component.childeren_count >= 2)
			{
				Entity& previous_sibling	= child_relation_component.previous;
				Entity& next_sibling		= child_relation_component.next;

				bool const is_previous_sibling_valid	= previous_sibling.IsEntityValid();
				bool const is_next_sibling_valid		= next_sibling.IsEntityValid();

				if (is_previous_sibling_valid && !is_next_sibling_valid)
				{
					// From: 'Previous' <-> 'Removed' | To: 'Previous' <-> Empty.
					previous_sibling.GetComponent<EntityRelationComponent>().next = Entity();
				}
				else if (is_previous_sibling_valid && is_next_sibling_valid)
				{
					// From: 'Previous' <-> 'Removed' <-> 'Next' | To: 'Previous' <-> 'Next'.
					previous_sibling.GetComponent<EntityRelationComponent>().next = next_sibling;
					next_sibling.GetComponent<EntityRelationComponent>().previous = previous_sibling;
				}
				else if (!is_previous_sibling_valid && is_next_sibling_valid)
				{
					// From: 'Removed' <-> 'Next' | To: 'Empty' <-> 'Next'.
					next_sibling.GetComponent<EntityRelationComponent>().previous = Entity();
				}

				// Wipe sibling data of the child.
				child_relation_component.previous	= Entity();
				child_relation_component.next		= Entity();
			}

			// Adjust parential data of child.
			child_relation_component.parent = Entity();

			// Reset hierarchy depth value on the removed child.
			child_relation_component.hierarchy_depth = DFW::DMath::uint16_max;

			// Update parent's children count.
			relation_component.childeren_count--;
		}

		void Entity::SetParent(Entity a_parent)
		{
			if (!a_parent.IsEntityValid())
				return;
			
			if (this->GetRegistry() != a_parent.GetRegistry())
				return;

			// Check if already is a child of the parent.
			// Also, prevents parents from becoming a child of one of its childeren.
			if (Detail::CheckForEntityInHierachy(*this, a_parent))
				return;

			EntityRelationComponent& relation_component = AddComponent<EntityRelationComponent>();
			if (relation_component.parent.IsEntityValid())
				relation_component.parent.RemoveChild(*this);

			a_parent.AddChild(*this);
		}

		bool Entity::IsParentOfEntity(Entity const& a_child) const
		{
			// If no relation component, then there are no relationships with this entity.
			EntityRelationComponent const* child_relation_component = a_child.TryGetComponent<EntityRelationComponent>();
			if (!child_relation_component)
				return false;

			if (child_relation_component->parent != *this)
				return false;

			return true;
		}

		bool Entity::IsChildOfEntity(Entity const& a_parent) const
		{
			// If no relation component, then there are no relationships with this entity.
			EntityRelationComponent const* relation_component = TryGetComponent<EntityRelationComponent>();
			if (!relation_component)
				return false;

			if (relation_component->parent != a_parent)
				return false;

			return true;
		}

		bool Entity::IsChildInEntityHierachyOf(Entity const& a_parent) const
		{
			return Detail::CheckForEntityInHierachy(a_parent, *this);
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
