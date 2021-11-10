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
			, _universe(nullptr)
		{
		}

		Entity::Entity(EntityHandle const& a_entity_handle, Universe* const a_universe)
			: _handle(a_entity_handle)
			, _universe(a_universe)
		{
		}

		std::strong_ordering Entity::operator<=>(Entity const& a_other) const
		{
			if (auto comparison = this->_handle <=> a_other._handle; comparison != 0)
				return comparison;

			if (this->_universe || a_other._universe)
				if (auto comparison = this->_universe <=> a_other._universe; comparison != 0)
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
			if (_handle != DFW_NULL_ENTITY_HANDLE)
				return _universe->_entity_data_registration[_handle]->id;

			return DFW::DUID();
		}

		EntityHandle Entity::GetHandle() const
		{
			return _handle;
		}

		Universe* Entity::GetUniverse() const
		{
			return _universe;
		}

		bool Entity::IsEntityValid() const
		{
			if (_handle == DFW_NULL_ENTITY_HANDLE)
				return false;

			if (_universe == nullptr)
				return false;

			if (!_universe->_registry.valid(_handle))
				return false;

			return true;
		}

		bool Entity::IsPendingDeletion() const
		{
			if (_handle != DFW_NULL_ENTITY_HANDLE)
			{
				auto const it = std::find(_universe->_deleted_entities.begin(), _universe->_deleted_entities.end(), _handle);
				if (it != _universe->_deleted_entities.end())
					return true;
			}

			return false;
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
