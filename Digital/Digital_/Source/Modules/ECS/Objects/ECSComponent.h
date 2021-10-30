#pragma once

#include <Autofactory/AutoFactory.h>

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSEntity.h>

namespace DECS 
{
	// FW Declare
	class ECSComponentManager;

	class ECSComponent : public DFactory::AutoFactory<ECSComponent> 
	{
	public:
		ECSComponent(Key);
		virtual ~ECSComponent() = default;
		
		ECSEntity GetOwner() const;
		DCore::DUID GetID() const;

	protected:
		friend class ECSComponentManager;

		DCore::DUID	_id;
		ECSEntity	_owner;

	};

} // End of namespace ~ DECS
