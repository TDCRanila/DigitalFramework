#include <Modules/ECS/Managers/ECSystemManager.h>

#include <AutoFactory/Autofactory.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/ECSModule.h>

#include <ranges>

namespace DECS 
{

	ECSystemManager::ECSystemManager() 
	{ 
		_systems.reserve(DFW_SYSTEM_RESERVE_AMOUNT);
	}

	void ECSystemManager::Init()
	{
	}

	void ECSystemManager::Terminate() 
	{ 
		// Calling Terminiate of systems.
		for (auto const&[system_type, system_ptr] : _systems)
		{
			system_ptr->InternalTerminate();
		}
	}

	void ECSystemManager::UpdateSystems(ECSUniverse* const a_universe)
	{ 
		DFW_ASSERT(a_universe && "Updating ECS Systems, but universe is invalid.");

		if (_systems.empty())
		{
			return;
		}

		auto IsSystemPaused = [](std::shared_ptr<ECSystem> const& system) -> bool
		{
			return system->IsSystemPaused();
		};

		// auto systems_values = std::views::values(_systems);
		auto enabled_system = std::views::values(_systems) | std::views::filter(IsSystemPaused);

		// Calling Init of systems.
		for (auto const&[system_type, system_ptr] : _systems)
		{
			system_ptr->InternalPreUpdate(a_universe);
		}

		// Calling Init of systems.
		for (auto const&[system_type, system_ptr] : _systems)
		{
			system_ptr->InternalUpdate(a_universe);
		}

		// Calling Init of systems.
		for (auto const&[system_type, system_ptr] : _systems)
		{
			system_ptr->InternalPostUpdate(a_universe);
		}
	}

} // End of namespace ~ DECS
