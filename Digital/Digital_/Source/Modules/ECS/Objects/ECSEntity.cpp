#include <Modules/ECS/Objects/ECSEntity.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/ECSEntityManager.h>

namespace DECS 
{
	ECSEntity::ECSEntity()
		: _handle(DFW_NULL_ENTITY_HANDLE)
		, _universe(nullptr)
	{ 
	}

	ECSEntity::ECSEntity(EntityHandle const& a_entity_handle, ECSUniverse* const a_universe)
		:	_handle(a_entity_handle)
		,	_universe(a_universe)
	{
	}

	std::strong_ordering ECSEntity::operator<=>(ECSEntity const& a_other) const
	{
		if (auto comparison = this->_handle <=> a_other._handle; comparison != 0)
			return comparison;

		if (this->_universe || a_other._universe)
			if (auto comparison = this->_universe <=> a_other._universe; comparison != 0)
				return comparison;

		return std::strong_ordering();
	}

	ECSEntity::operator EntityHandle()
	{ 
		return _handle;
	}

	ECSEntity::operator EntityHandle() const
	{ 
		return _handle;
	}

	DCore::DUID ECSEntity::GetID() const 
	{
		if (_handle != DFW_NULL_ENTITY_HANDLE)
			return _universe->_entity_data_registration[_handle]->id;

		return DCore::DUID();
	}

	EntityHandle ECSEntity::GetHandle() const
	{
		return _handle;
	}

	ECSUniverse* ECSEntity::GetUniverse() const 
	{
		return _universe;
	}

	bool ECSEntity::IsEntityValid() const
	{
		if (_handle == DFW_NULL_ENTITY_HANDLE)
			return false;

		if (_universe == nullptr)
			return false;

		if (!_universe->_registry.valid(_handle))
			return false;

		return true;
	}

	bool ECSEntity::IsPendingDeletion() const
	{
		if (_handle != DFW_NULL_ENTITY_HANDLE)
		{
			auto const it = std::find(_universe->_deleted_entities.begin(), _universe->_deleted_entities.end(), _handle);
			if (it != _universe->_deleted_entities.end())
				return true;
		}

		return false;
	}

} // End of namespace ~ DECS
