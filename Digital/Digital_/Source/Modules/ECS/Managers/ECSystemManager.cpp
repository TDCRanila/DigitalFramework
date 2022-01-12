#include <Modules/ECS/Managers/ECSystemManager.h>

#include <AutoFactory/Autofactory.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/ECSModule.h>

#include <ranges>

namespace DFW
{
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
			for (auto const& [system_type, system_ptr] : _systems)
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

			// Calling Init of systems.
			for (auto const& [system_type, system_ptr] : _systems)
			{
				if (!system_ptr->IsSystemPaused())
					system_ptr->InternalPreUpdate(a_universe);
			}

			// Calling Init of systems.
			for (auto const& [system_type, system_ptr] : _systems)
			{
				if (!system_ptr->IsSystemPaused())
					system_ptr->InternalUpdate(a_universe);
			}
			
			// Calling Init of systems.
			for (auto const& [system_type, system_ptr] : _systems)
			{
				if (!system_ptr->IsSystemPaused())
					system_ptr->InternalPostUpdate(a_universe);
			}
		}

		void SystemManager::UpdateSystemsImGui(Universe* const a_universe)
		{
			if (_systems.empty())
			{
				return;
			}

			for (auto const& [system_type, system_ptr] : _systems)
			{
				system_ptr->UpdateSystemImGui(a_universe);
			}
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
