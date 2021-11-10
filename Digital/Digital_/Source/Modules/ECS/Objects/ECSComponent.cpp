#include <Modules/ECS/Objects/ECSComponent.h>

namespace DECS
{

	Component::Component(Key)
		: _owner()
		, _id(DCore::DFW_INVALID_DUID)
	{ 
	}

	Entity Component::GetOwner() const
	{
		return _owner;
	}

	DCore::DUID Component::GetID() const 
	{
		return _id;
	}

} // End of namespace ~ DECS
