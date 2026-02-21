#include <DFW/Modules/ECS/Utility/ComponentBitListUtility.h>

#include <DFW/Modules/ECS/Component.h>

#include <DFW/CoreSystems/Logging/Logger.h>

#include <DFW/Utility/AutoFactory/AutoFactory.h>

namespace DFW
{
	namespace DECS
	{
		bool ComponentBitListUtility::_has_generated_component_keys = false;
		std::unordered_map<std::type_index, int8> ComponentBitListUtility::_component_bit_placement;

		void ComponentBitListUtility::GenerateComponentKeys()
		{
			if (_has_generated_component_keys)
			{
				DFW_INFOLOG("ComponentBitListUtility has already generated the ComponentBitLists.");
				return;
			}

			int8 key_index = -1;

			for (auto const& [type_pair, fac] : Component::GetFactories())
			{
				_component_bit_placement[type_pair.second] = ++key_index;

				if (key_index > DFW_MAX_REGISTERED_COMPONENTS)
				{
					DFW_ERRORLOG("Over {} Components have been registered in ComponentBitListUtility.", DFW_MAX_REGISTERED_COMPONENTS);
				}
			}

			_has_generated_component_keys = true;
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
