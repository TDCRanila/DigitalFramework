#include <ECS/Objects/ECSComponent.h>

#include <ECS/Utility/ECSIDManager.h>

namespace DECS
{

	ECSComponent::ECSComponent(Key) :
		component_id_(ECSIDManager::GetNextComponentID())
	{ 
		/*EMPTY*/ 
	}

	ECSComponent::~ECSComponent() { /*EMPTY*/ }

	ComponentID ECSComponent::GetID() const 
	{
		return component_id_;
	}

	EntityID ECSComponent::GetOwner() const
	{
		return owner_id_;
	}

} // End of namespace ~ DECS
