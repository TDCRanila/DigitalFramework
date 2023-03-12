#pragma once

#include <Modules/ECS/Entity.h>
#include <Modules/ECS/Internal/InternalEntity.h>
#include <Modules/ECS/Internal/InternalComponent.h>

#include <Utility/AutoFactory/AutoFactory.h>

namespace DFW
{
	namespace DECS
	{
		class Component : public InternalComponent, public DFactory::AutoFactory<Component>
		{
		public:
			Component(Key);
			virtual ~Component() = default;

			Entity GetOwner() const { return Entity(_owner.GetHandle(), _owner.GetRegistry()); }

		};

	} // End of namespace ~ DECS.

	using Component = DECS::Component;

} // End of namespace ~ DFW.
