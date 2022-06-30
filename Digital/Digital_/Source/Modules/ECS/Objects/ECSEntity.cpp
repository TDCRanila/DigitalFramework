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
			, _universe(nullptr)
		{
		}

		Entity::Entity(EntityHandle a_entity_handle, Universe& a_universe)
			: _handle(a_entity_handle)
			, _universe(&a_universe)
		{
			DFW_ASSERT(a_universe.IsValid());
			EntityDataComponent const& comp = a_universe._entity_handle_registration.at(_handle);
			_id = comp.id;
		}

		std::strong_ordering Entity::operator<=>(Entity const& a_other) const
		{
			if (auto comparison = this->_handle <=> a_other._handle; comparison != 0)
				return comparison;

			if (auto comparison = (this->_universe <=> a_other._universe); comparison != 0)
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

		Universe& Entity::GetUniverse() const
		{
			DFW_ASSERT(_universe);
			return (*_universe);
		}

		bool Entity::IsEntityValid() const
		{
			if (_handle == DFW_NULL_ENTITY_HANDLE)
				return false;

			if (!_universe)
				return false;

			if (!_universe->registry.valid(_handle))
				return false;

			return true;
		}

		bool Entity::IsPendingDeletion() const
		{
			DFW_ASSERT(_universe);
			return _universe->_pending_deletion_entities.end() != _universe->_pending_deletion_entities.find(_handle);
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
