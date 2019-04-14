#include <ECS/Managers/ECSystemManager.h>

#include <ECS/Utility/ECSIDManager.h>

#include <AutoFactory/Autofactory.h>

namespace DECS 
{

	ECSystemManager::ECSystemManager() { /*EMPTY*/ }

	ECSystemManager::~ECSystemManager() { /*EMPTY*/ }

	void ECSystemManager::Init() 
	{
		// Create the starting list of systems.
		this->SetupSystemList();
	}

	void ECSystemManager::Terminate() { /*EMPTY*/ }

	void ECSystemManager::UpdateSystems() { /*EMPTY*/ }

	void ECSystemManager::AddSystemInternal(std::type_index a_type_index, std::unique_ptr<ECSystem> a_system_ptr) 
	{
		// TODO: Set System IDs
		systems_[a_type_index] = std::move(a_system_ptr);
	}

	void ECSystemManager::SetupSystemList() 
	{
		for (auto const&[type_pair, fac] : ECSystem::GetFactories()) 
		{
			this->AddSystemInternal(type_pair.second, fac());
		}
	}

	void ECSystemManager::SortSystemPriority() { /*EMPTY*/ }


} // End of namespace ~ DECS
