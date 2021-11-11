#include <Modules/ECS/Objects/ECSComponent.h>

namespace DFW
{
	namespace DECS
	{
		Component::Component(Key)
			: _owner()
			, _id(DFW::DFW_INVALID_DUID)
		{
		}

		Entity Component::GetOwner() const
		{
			return _owner;
		}

		DFW::DUID Component::GetID() const
		{
			return _id;
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
