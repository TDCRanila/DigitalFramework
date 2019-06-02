#include <ECS/Managers/ECSystemManager.h>

#include <ECS/Utility/ECSIDManager.h>

#include <AutoFactory/Autofactory.h>

namespace DECS 
{

	ECSystemManager::ECSystemManager() { /*EMPTY*/ }

	ECSystemManager::~ECSystemManager() { /*EMPTY*/ }

	void ECSystemManager::Init(ECSEntityManager* a_entity_manager)
	{
		// Create the starting list of systems.
		this->SetupSystemList();

		// Calling Init of systems.
		for (auto const&[system_type, system_ptr] : systems_)
		{
			system_ptr->InternalInit(a_entity_manager);
		}
	}

	void ECSystemManager::Terminate() 
	{ 
		// Calling Terminiate of systems.
		for (auto const&[system_type, system_ptr] : systems_)
		{
			system_ptr->InternalTerminate();
		}
	}

	void ECSystemManager::UpdateSystems() 
	{ 
		// Calling Init of systems.
		for (auto const&[system_type, system_ptr] : systems_)
		{
			system_ptr->InternalPreUpdate();
		}

		// Calling Init of systems.
		for (auto const&[system_type, system_ptr] : systems_)
		{
			system_ptr->InternalUpdate();
		}

		// Calling Init of systems.
		for (auto const&[system_type, system_ptr] : systems_)
		{
			system_ptr->InternalPostUpdate();
		}
	}

	void ECSystemManager::SetupSystemList() 
	{
		for (auto const&[type_pair, fac] : ECSystem::GetFactories()) 
		{
			this->AddSystemInternal(type_pair.second, fac());
		}
	}

	void ECSystemManager::SortSystemPriority() { /*EMPTY*/ }

	void ECSystemManager::AddSystemInternal(std::type_index a_type_index, std::unique_ptr<ECSystem> a_system_ptr) 
	{
		// TODO: Set System IDs
		systems_[a_type_index] = std::move(a_system_ptr);
	}

} // End of namespace ~ DECS
