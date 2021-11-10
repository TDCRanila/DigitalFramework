#include <Modules/ECS/Managers/ECSystemManager.h>

#include <AutoFactory/Autofactory.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/ECSModule.h>

#include <ranges>

namespace DECS 
{

	SystemManager::SystemManager() 
	{ 
		_systems.reserve(DFW_SYSTEM_RESERVE_AMOUNT);
	}

	void SystemManager::Init()
	{
	}

	void SystemManager::Terminate() 
	{ 
		// Calling Terminiate of systems.
		for (auto const&[system_type, system_ptr] : _systems)
		{
			system_ptr->InternalTerminate();
		}
	}

	void SystemManager::UpdateSystems(Universe* const a_universe)
	{ 
		DFW_ASSERT(a_universe && "Updating DECS Systems, but universe is invalid.");

		if (_systems.empty())
		{
			return;
		}

		auto IsSystemPaused = [](std::shared_ptr<System> const& system) -> bool
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
