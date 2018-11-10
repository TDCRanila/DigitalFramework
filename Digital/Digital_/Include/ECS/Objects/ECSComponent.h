#pragma once

#include <Autofactory/AutoFactory.h>

#include <ECS/Utility/ECSIDManager.h>

#include <stdint.h>

namespace DECS {

	//class ECSComponentManager;

	class ECSComponent : public DFactory::AutoFactory<ECSComponent> {
	public:
		ECSComponent(Key);
		virtual ~ECSComponent();

		EntityID GetOwner() const;
		ComponentID GetID() const;

	protected:
		//friend ECSComponentManager;
		bool IsOwned() const;

	private:
		EntityID owned_id_;
		
		ComponentID component_id_;

	};

} // End of namespace ~ DECS
