#pragma once

#include <Autofactory/AutoFactory.h>

#include <Modules/ECS/Utility/ECSIDManager.h>

namespace DECS 
{
	class ECSComponent : public DFactory::AutoFactory<ECSComponent> 
	{
	public:
		ECSComponent(Key);
		virtual ~ECSComponent();
		
		EntityID GetOwner() const;

		ComponentID GetID() const;

	private:
		EntityID	owner_id_;
		ComponentID component_id_;

	};

} // End of namespace ~ DECS
