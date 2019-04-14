#include <ECS/Utility/ECSKeyLockSystem.h>

#include <ECS/Objects/ECSComponent.h>
#include <AutoFactory/AutoFactory.h>

namespace DECS 
{

	ECSKeyLockSystem::ECSKeyLockSystem() { /*EMPTY*/ }
	ECSKeyLockSystem::~ECSKeyLockSystem() { /*EMPTY*/ }

	void ECSKeyLockSystem::GenerateComponentKeys() 
	{
		int8_t key_index = 0;

		for (auto const&[type_pair, fac] : ECSComponent::GetFactories()) 
		{
			component_bit_placement_[type_pair.second] = key_index++;

			if (key_index > 64) 
			{ 
				ERRORLOG("Over 64 Components have been registered for ComponentBitPlacement.") 
			}
		}
	}

} // End of namespace ~ DECS
