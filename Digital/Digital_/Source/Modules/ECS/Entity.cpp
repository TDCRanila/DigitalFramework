#include <Modules/ECS/Entity.h>

#include <Modules/ECS/Internal/EntityDataComponent.h>

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
			return GetComponent<EntityDataComponent>().id;
		}

		EntityTypeID Entity::GetTypeID() const
		{
			return GetComponent<EntityDataComponent>().type;
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

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
