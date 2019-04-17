#include <ECS/Utility/ECSKeyLockSystem.h>

#include <ECS/Objects/ECSComponent.h>
#include <AutoFactory/AutoFactory.h>

namespace DECS 
{

	ECSKeyLockSystem::ECSKeyLockSystem() :
		generated_component_keys_(false)
	{ 
		/*EMPTY*/ 
	}

	ECSKeyLockSystem::~ECSKeyLockSystem() { /*EMPTY*/ }

	void ECSKeyLockSystem::GenerateComponentKeys() 
	{
		if (generated_component_keys_)
		{
			INFOLOG("KeyLockSystem has already generated the component keys.");
			return;
		}

		int8 key_index = -1;

		for (auto const&[type_pair, fac] : ECSComponent::GetFactories()) 
		{
			component_bit_placement_[type_pair.second] = ++key_index;

			if (key_index > 64) 
			{ 
				ERRORLOG("Over 64 Components have been registered for ComponentBitPlacement.") 
			}
		}

		generated_component_keys_ = true;
	}

} // End of namespace ~ DECS
