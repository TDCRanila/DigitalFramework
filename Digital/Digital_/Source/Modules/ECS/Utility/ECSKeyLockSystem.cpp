#include <Modules/ECS/Utility/ECSKeyLockSystem.h>

#include <AutoFactory/AutoFactory.h>

#include <Modules/ECS/Objects/ECSComponent.h>

#include <CoreSystems/Logging/Logger.h>

namespace DECS 
{
	bool KeyLockSystem::_generated_component_keys = false;
	std::unordered_map<std::type_index, int8> KeyLockSystem::_component_bit_placement;

	void KeyLockSystem::GenerateComponentKeys() 
	{
		if (_generated_component_keys)
		{
			DFW_INFOLOG("KeyLockSystem has already generated the component keys.");
			return;
		}

		int8 key_index = -1;

		for (auto const&[type_pair, fac] : Component::GetFactories()) 
		{
			_component_bit_placement[type_pair.second] = ++key_index;

			if (key_index > DFW_MAX_REGISTERED_COMPONENTS)
			{ 
				DFW_ERRORLOG("Over {} Components have been registered for ComponentBitPlacement.", DFW_MAX_REGISTERED_COMPONENTS);
			}
		}

		_generated_component_keys = true;
	}

} // End of namespace ~ DECS
