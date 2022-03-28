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
		private:
			friend ComponentManager;

		public:
			Component(Key);
			virtual ~Component() = default;

			Entity const& GetOwner();
			DFW::DUID GetID() const;

		private:
			DFW::DUID	_id;
			Entity		_owner;

		};

	} // End of namespace ~ DECS.

	using Component = DECS::Component;

} // End of namespace ~ DFW.
