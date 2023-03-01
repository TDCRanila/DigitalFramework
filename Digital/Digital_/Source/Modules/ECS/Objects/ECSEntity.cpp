#include <Modules/ECS/Objects/ECSEntity.h>

#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>

namespace DFW
{
	namespace DECS
	{
		Entity::Entity(EntityHandle a_entity_handle, EntityRegistry& a_registry)
			: InternalEntity(a_entity_handle, &a_registry)
		{
		}

		std::strong_ordering Entity::operator<=>(Entity const& a_other) const
		{
			if (auto comparison = this->_handle <=> a_other._handle; comparison != 0)
				return comparison;

			if (auto comparison = (this->_registry <=> a_other._registry); comparison != 0)
					return comparison;

			return std::strong_ordering();
		}

		DFW::DUID Entity::GetID() const
		{
			return _registry->registry.get<EntityDataComponent>(_handle).id;
		}

		EntityTypeID Entity::GetTypeID() const
		{
			return _registry->registry.get<EntityDataComponent>(_handle).type;
		}

		std::string Entity::GetName() const
		{
			return _registry->registry.get<EntityDataComponent>(_handle).name;
		}

		void Entity::SetName(std::string const& a_new_name)
		{
			_registry->registry.get<EntityDataComponent>(_handle).name = a_new_name;
		}

		bool Entity::IsPendingDeletion() const
		{
			DFW_ASSERT(_registry);
			return _registry->_pending_deletion_entities.end() != _registry->_pending_deletion_entities.find(_handle);
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
