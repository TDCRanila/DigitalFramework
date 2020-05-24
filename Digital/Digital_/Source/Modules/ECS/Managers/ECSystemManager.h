#pragma once

#include <Modules/ECS/Objects/ECSystem.h>

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace DECS 
{
	class ECSEntityManager;

	class ECSystemManager final 
	{
	public:
		ECSystemManager();
		~ECSystemManager();

	protected:
		friend class ECSModule;

		void Init(ECSEntityManager* a_entity_manager);

		void Terminate();

		void UpdateSystems();

		template <class T>
		void AddSystem();

		template <class T>
		void RemoveSystem();

		template <class T>
		void PauseSystem(bool a_pause_on_true);

	private:
		template <class T>
		bool IsSystemPresent();

		void SetupSystemList();
		void SortSystemPriority();

		void AddSystemInternal(std::type_index a_type_index, std::unique_ptr<ECSystem> a_system_ptr);

		std::unordered_map<std::type_index, std::unique_ptr<ECSystem>> systems_;

	};

#pragma region Template Function Implementation

	template <class T>
	void ECSystemManager::AddSystem()
	{
		// TODO.
	}

	template <class T>
	void ECSystemManager::RemoveSystem()
	{
		// TODO.
	}

	template <class T>
	void ECSystemManager::PauseSystem(bool a_pause_on_true)
	{
		auto& type				= typeid(T);
		bool is_system_present	= IsSystemPresent<T>();
		if (is_system_present)
		{
			systems_[type]->InternalPauseSystem(a_pause_on_true);
		}
		else
		{
			ERRORLOG("Unable to pause System of T: " << type.name());
			return;
		}
	}

	template <class T>
	bool ECSystemManager::IsSystemPresent()
	{
		auto& type	= typeid(T);
		auto it		= systems_.find(type);
		if (it != systems_.end())
		{
			return false;
		}
		else
		{
			ERRORLOG("Unable to find System of T: " << type.name());
			return true;
		}
	}

#pragma endregion 

} // End of namespace ~ DECS
