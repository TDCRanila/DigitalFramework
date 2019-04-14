#pragma once

#include <ECS/Objects/ECSystem.h>

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace DECS 
{

	class ECSystemManager final 
	{
	public:
		ECSystemManager();
		~ECSystemManager();

		void Init();
		void Terminate();

		void UpdateSystems();

		template <class T>
		void AddSystem();

		template <class T>
		void RemoveSystem();

		template <class T>
		void PauseSystem();

	private:
		void AddSystemInternal(std::type_index a_type_index, std::unique_ptr<ECSystem> a_system_ptr);
		
		void SetupSystemList();
		void SortSystemPriority();

		std::unordered_map<std::type_index, std::unique_ptr<ECSystem>> systems_;

	};

} // End of namespace ~ DECS
