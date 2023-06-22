#include <Modules/ECS/Managers/SystemManager.h>

namespace DFW
{
	namespace DECS
	{
		SystemManager::SystemManager(ECSModule const* a_ecs)
			: _ecs(a_ecs)
		{
		}

		SystemManager::~SystemManager() = default;

		void SystemManager::RemoveAllSystems(EntityRegistry& a_registry)
		{
			// Calling Terminiate of systems.
			for (auto const& [system_type, system_ptr] : _systems)
				system_ptr->InternalTerminate(a_registry);

			_system_providing_dependencies_map.clear();
			_system_relying_dependencies_map.clear();
			_system_execution_list.clear();
			_systems.clear();
		}

		void SystemManager::Terminate(EntityRegistry& a_registry)
		{
			RemoveAllSystems(a_registry);
		}

		void SystemManager::UpdateSystems(EntityRegistry& a_registry)
		{
			// Calling Init of systems.
			for (System* system : _system_execution_list)
			{
				if (!system->IsSystemPaused())
					system->InternalPreUpdate(a_registry);
			}

			// Calling Init of systems.
			for (System* system : _system_execution_list)
			{
				if (!system->IsSystemPaused())
					system->InternalUpdate(a_registry);
			}
			
			// Calling Init of systems.
			for (System* system : _system_execution_list)
			{
				if (!system->IsSystemPaused())
					system->InternalPostUpdate(a_registry);
			}
		}

		void SystemManager::UpdateSystemsImGui(EntityRegistry& a_registry)
		{
			for (auto const& system_ptr : _system_execution_list)
				system_ptr->UpdateSystemImGui(a_registry);
		}

		void SystemManager::AddSystemDependency(System const& a_relying_system, System const& a_providing_system)
		{
			SystemTypeID const relying_system_id = a_relying_system.GetTypeID();
			SystemTypeID const providing_system_id = a_providing_system.GetTypeID();

			std::vector<SystemTypeID>& system_relying_dependencies = _system_relying_dependencies_map[relying_system_id];
			system_relying_dependencies.emplace_back(providing_system_id);

			std::vector<SystemTypeID>& system_providing_dependencies = _system_providing_dependencies_map[providing_system_id];
			system_providing_dependencies.emplace_back(relying_system_id);
		}

		void SystemManager::RemoveSystemDependency(System const& a_relying_system, System const& a_providing_system)
		{
			SystemTypeID const relying_system_id = a_relying_system.GetTypeID();
			SystemTypeID const providing_system_id = a_providing_system.GetTypeID();

			if (auto const it = _system_relying_dependencies_map.find(relying_system_id); it != _system_relying_dependencies_map.end())
			{
				std::vector<SystemTypeID>& outgoing_dependencies = it->second;
				std::erase_if(outgoing_dependencies, [&providing_system_id](SystemTypeID const& a_system_type_id) { return a_system_type_id == providing_system_id; });

				if (outgoing_dependencies.empty())
					_system_relying_dependencies_map.erase(it);
			}

			if (auto const it = _system_providing_dependencies_map.find(providing_system_id); it != _system_providing_dependencies_map.end())
			{
				std::vector<SystemTypeID>& incoming_dependencies = it->second;
				std::erase_if(incoming_dependencies, [&relying_system_id](SystemTypeID const& a_system_type_id) { return a_system_type_id == relying_system_id; });

				if (incoming_dependencies.empty())
					_system_providing_dependencies_map.erase(it);
			}
		}

		void SystemManager::CalculateSystemDependencies()
		{		
			// Sorting function based on Kahn's algorithm to determine which systems should be executed
			// based on their dependencies.
			auto topological_dependency_sort = [this](std::vector<SystemTypeID> const& a_starting_system) -> std::vector<SystemTypeID>
			{
				std::vector<SystemTypeID> sorted_systems;
				sorted_systems.reserve(this->_systems.size());
				std::vector<SystemTypeID> processing_systems;
				auto relying_dependencies_map = this->_system_relying_dependencies_map;

				for (SystemTypeID const& system : a_starting_system)
				{
					processing_systems.emplace_back(system);
					
					while (!processing_systems.empty())
					{
						SystemTypeID const& providing_system = sorted_systems.emplace_back(*processing_systems.begin());
						processing_systems.erase(processing_systems.begin());

						// Find if any RelyingSystems are dependant on this ProvidingSystem.
						auto const providing_dependency_it = this->_system_providing_dependencies_map.find(providing_system);
						if (providing_dependency_it == this->_system_providing_dependencies_map.end())
							continue;

						// Remove all dependencies from the RelyingSystems that relies on the ProvidingSystem.
						std::vector<SystemTypeID> const& providing_dependencies = providing_dependency_it->second;
						for (SystemTypeID const& relying_system : providing_dependencies)
						{
							auto& relying_dependencies = relying_dependencies_map.at(relying_system);
							relying_dependencies.erase(std::find(relying_dependencies.begin(), relying_dependencies.end(), providing_system));

							// If the RelyingSystem has no more relying dependencies, add it to the list of processing nodes.
							if (relying_dependencies_map.at(relying_system).empty())
								processing_systems.emplace_back(relying_system);
						}
					}
				}

				DFW_ASSERT(!relying_dependencies_map.empty() && "Detected cycles in the system dependency graphs as dependencies are still present.");
				return sorted_systems;
			};

			// Find systems that have no dependencies on other systems.
			// Sorted by insertion order of systems.
			std::vector<SystemTypeID> starting_systems;
			for (auto const& [system_type, system_ptr] : _systems)
			{
				auto const it = _system_relying_dependencies_map.find(system_type);
				if (it == _system_relying_dependencies_map.end() || it->second.empty())
					starting_systems.emplace_back(system_type);
			}

			// Sort systems based on dependencies.
			std::vector<SystemTypeID> sorted_system_dependency_list = topological_dependency_sort(starting_systems);
			DFW_ASSERT(sorted_system_dependency_list.size() == _systems.size() && "Too many or missing systems in the dependency list.");

			// Make an system execution list based on the sorted list.
			_system_execution_list.clear();
			_system_execution_list.reserve(_systems.size());
			for (SystemTypeID const& system : sorted_system_dependency_list)
				_system_execution_list.emplace_back(_systems.at(system).get());

		}

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
