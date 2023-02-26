#include <Modules/ECS/Objects/ECSEntity.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/ECSEntityManager.h>

namespace DFW
{
	namespace DECS
	{
		Entity::Entity()
			: _handle(DFW_NULL_ENTITY_HANDLE)
			, _id(DFW::DFW_INVALID_DUID)
			, _registry(nullptr)
		{
		}

		Entity::Entity(EntityHandle a_entity_handle, EntityRegistry& a_registry)
			: _handle(a_entity_handle)
			, _registry(&a_registry)
		{
			DFW_ASSERT(a_registry.IsValid());
			EntityDataComponent const& comp = a_registry._entity_handle_registration.at(_handle);
			_id = comp.id;
		}

		std::strong_ordering Entity::operator<=>(Entity const& a_other) const
		{
			if (auto comparison = this->_handle <=> a_other._handle; comparison != 0)
				return comparison;

			if (auto comparison = (this->_registry <=> a_other._registry); comparison != 0)
					return comparison;

			return std::strong_ordering();
		}

		Entity::operator EntityHandle()
		{
			return _handle;
		}

		Entity::operator EntityHandle() const
		{
			return _handle;
		}

		DFW::DUID Entity::GetID() const
		{
			return _id;
		}

		EntityHandle Entity::GetHandle() const
		{
			return _handle;
		}

		EntityRegistry& Entity::GetRegistry() const
		{
			DFW_ASSERT(_registry);
			return (*_registry);
		}

		bool Entity::IsEntityValid() const
		{
			if (_handle == DFW_NULL_ENTITY_HANDLE)
				return false;

			if (!_registry)
				return false;

			if (!_registry->registry.valid(_handle))
				return false;

			return true;
		}

		bool Entity::IsPendingDeletion() const
		{
			DFW_ASSERT(_registry);
			return _registry->_pending_deletion_entities.end() != _registry->_pending_deletion_entities.find(_handle);
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
