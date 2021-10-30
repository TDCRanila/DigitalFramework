#pragma once

#include <AutoFactory/AutoFactory.h>

#include <CoreSystems/DUID.h>

#include <entt/entity/handle.hpp>

namespace DECS 
{
	// FW Declare.
	class ECSUniverse;
	class ECSEntityManager;
	class ECSComponentManager;
	
	using EntityHandle								= entt::entity;
	constexpr entt::null_t DFW_NULL_ENTITY_HANDLE	= entt::null;

	class ECSEntity : public DFactory::AutoFactory<ECSEntity>
	{
	public:
		ECSEntity();
		ECSEntity(EntityHandle const& a_entity_handle, ECSUniverse* const a_universe);
		virtual ~ECSEntity() = default;
		
		std::strong_ordering operator<=>(ECSEntity const& a_other) const;

		operator EntityHandle();
		operator EntityHandle() const;
		
		DCore::DUID GetID() const;
		EntityHandle GetHandle() const;
		ECSUniverse* GetUniverse() const;

		bool IsEntityValid() const;
		bool IsPendingDeletion() const;

	private:
		friend ECSEntityManager;
		friend ECSComponentManager;

		EntityHandle	_handle;
		ECSUniverse*	_universe;

	};

} // End of namespace ~ DECS
