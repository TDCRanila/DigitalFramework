#pragma once

#include <Autofactory/AutoFactory.h>

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSEntity.h>

namespace ECS 
{
	// FW Declare
	class ComponentManager;

	class Component : public DFactory::AutoFactory<Component> 
	{
	public:
		Component(Key);
		virtual ~Component() = default;
		
		Entity GetOwner() const;
		DCore::DUID GetID() const;

	protected:
		friend class ComponentManager;

		DCore::DUID	_id;
		Entity	_owner;

	};

} // End of namespace ~ ECS
