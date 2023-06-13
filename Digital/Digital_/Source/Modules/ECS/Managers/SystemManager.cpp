#include <Modules/ECS/Managers/SystemManager.h>

#include <Modules/ECS/Managers/EntityRegistry.h>

#include <Utility/AutoFactory/AutoFactory.h>

#include <ranges>

namespace DFW
{
	namespace DECS
	{
		SystemManager::SystemManager(ECSModule const* a_ecs)
		{
			_systems.reserve(DFW_SYSTEM_RESERVE_AMOUNT);
			_ecs = a_ecs;
		}

		SystemManager::~SystemManager() = default;

		void SystemManager::Init(EntityRegistry& a_registry)
		{
			// Calling Init of systems.
			for (auto const& [system_type, system_ptr] : _systems)
			{
				system_ptr->InternalInit(a_registry);
			}
		}

		void SystemManager::Terminate(EntityRegistry& a_registry)
		{
			// Calling Terminiate of systems.
			for (auto const& [system_type, system_ptr] : _systems)
			{
				system_ptr->InternalTerminate(a_registry);
			}
		}

		void SystemManager::UpdateSystems(EntityRegistry& a_registry)
		{
			if (_systems.empty())
			{
				return;
			}

			// Calling Init of systems.
			for (auto const& [system_type, system_ptr] : _systems)
			{
				if (!system_ptr->IsSystemPaused())
					system_ptr->InternalPreUpdate(a_registry);
			}

			// Calling Init of systems.
			for (auto const& [system_type, system_ptr] : _systems)
			{
				if (!system_ptr->IsSystemPaused())
					system_ptr->InternalUpdate(a_registry);
			}
			
			// Calling Init of systems.
			for (auto const& [system_type, system_ptr] : _systems)
			{
				if (!system_ptr->IsSystemPaused())
					system_ptr->InternalPostUpdate(a_registry);
			}
		}

		void SystemManager::UpdateSystemsImGui(EntityRegistry& a_registry)
		{
			if (_systems.empty())
			{
				return;
			}

			for (auto const& [system_type, system_ptr] : _systems)
			{
				system_ptr->UpdateSystemImGui(a_registry);
			}
		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
