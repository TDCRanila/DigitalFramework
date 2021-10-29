#include <Modules/ECS/Objects/ECSComponent.h>

namespace DECS
{

	ECSComponent::ECSComponent(Key)
		: _owner()
		, _id(DCore::InvalidDUID())
	{ 
	}

	ECSEntity ECSComponent::GetOwner() const
	{
		return _owner;
	}

	DCore::DUID ECSComponent::GetID() const 
	{
		return _id;
	}

} // End of namespace ~ DECS
