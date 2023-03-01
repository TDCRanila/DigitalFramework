#pragma once

#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/InternalEntity.h>
#include <Modules/ECS/Objects/InternalComponent.h>

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
