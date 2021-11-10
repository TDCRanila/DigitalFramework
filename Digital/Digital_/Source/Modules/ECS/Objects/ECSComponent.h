#pragma once

#include <Autofactory/AutoFactory.h>

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSEntity.h>

namespace DFW
{
	namespace DECS
	{
		// FW Declare
		class ComponentManager;

		class Component : public DFactory::AutoFactory<Component>
		{
		public:
			Component(Key);
			virtual ~Component() = default;

			Entity GetOwner() const;
			DFW::DUID GetID() const;

		protected:
			friend class ComponentManager;

			DFW::DUID	_id;
			Entity	_owner;

		};

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
