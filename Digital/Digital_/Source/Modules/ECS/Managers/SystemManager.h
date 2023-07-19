#pragma once

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/System.h>
#include <Modules/ECS/Utility/ECSTemplateUtility.h>

#include <CoreSystems/Memory.h>
#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Logging/Logger.h>

#include <unordered_map>

namespace DFW
{
	namespace DECS
	{
		// FW Declare
		class ECSModule;
		class EntityRegistry;

		class SystemManager final
		{
		private:
			friend ECSModule;

		public:
			SystemManager(ECSModule const* a_ecs);
			~SystemManager();

			template <typename SystemType, typename ...TArgs>
			requires IsValidSystemType<SystemType>
				SystemType& AddSystem(TArgs&&... a_args);

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				void RemoveSystem();

			void RemoveSystems(bool a_should_terminate_systems);

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				void EnableSystem();

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				void DisableSystem();

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				bool IsSystemDisabled() const;

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				SystemType* GetSystem();

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				bool IsSystemPresent() const;

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				SystemTypeID GetSystemTypeID() const;

		public:
			void AddSystemDependency(System const& a_relying_system, System const& a_providing_system);
			void RemoveSystemDependency(System const& a_relying_system, System const& a_providing_system);
			
			void CalculateSystemDependencies();

		private:
			void TerminateSystems(EntityRegistry& a_registry);

			void UpdateSystems(EntityRegistry& a_registry);
			void UpdateSystemsImGui(EntityRegistry& a_registry);

			using SystemMapIterator = std::unordered_map<SystemTypeID, SharedPtr<System>>::iterator;

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
			SystemMapIterator FindSystem();

		private:
			std::unordered_map<SystemTypeID, SharedPtr<System>> _systems;
			std::vector<System*> _system_execution_list;

			std::unordered_map<SystemTypeID, std::vector<SystemTypeID>> _system_relying_dependencies_map;
			std::unordered_map<SystemTypeID, std::vector<SystemTypeID>> _system_providing_dependencies_map;

			ECSModule const* _ecs;

		};

#pragma region Template Function Implementation

		template <typename SystemType, typename ...TArgs>
		requires IsValidSystemType<SystemType>
		SystemType& SystemManager::AddSystem(TArgs&&... a_args)
		{
			SystemTypeID const type_id = GetSystemTypeID<SystemType>();
			if (_systems.contains(type_id))
			{
				DFW_WARNLOG("Trying to add a new System that is already registered in the manager.");
				return *std::static_pointer_cast<SystemType>(_systems.at(type_id));
			}
			
			SharedPtr<SystemType> system_ptr = MakeShared<SystemType>(std::forward<TArgs>(a_args)...);

			system_ptr->_name			= typeid(SystemType).name();
			system_ptr->_id				= DFW::GenerateDUID();
			system_ptr->_type_id		= type_id;
			system_ptr->_system_manager	= this;
			system_ptr->_event_handler	= &_ecs->EventHandler();

			DFW_ASSERT(system_ptr->_event_handler);

			_systems.emplace(type_id, system_ptr);

			system_ptr->InternalInit(_ecs->Registry());

			return *std::static_pointer_cast<SystemType>(system_ptr);
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			void SystemManager::RemoveSystem()
		{
			SystemMapIterator const it = FindSystem<SystemType>();

			// Remove any dependencies that based on this system.
			if (_system_providing_dependencies_map.contains(it->first))
			{
				for (SystemTypeID const system_type : _system_providing_dependencies_map.at(it->first))
					RemoveSystemDependency(*it->second, *_systems.at(system_type));
			}

			if (_system_relying_dependencies_map.contains(it->first))
			{
				for (SystemTypeID const system_type : _system_relying_dependencies_map.at(it->first))
					RemoveSystemDependency(*it->second, *_systems.at(system_type));
			}

			it->second->InternalTerminate(_ecs->Registry());
			_systems.erase(it);
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			void SystemManager::EnableSystem()
		{
			SystemMapIterator const it = FindSystem<SystemType>();
			return (it->second)->InternalPauseSystem(false);
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			void SystemManager::DisableSystem()
		{
			SystemMapIterator const it = FindSystem<SystemType>();
			return (it->second)->InternalPauseSystem(true);
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			bool SystemManager::IsSystemDisabled() const
		{
			SystemMapIterator const it = FindSystem<SystemType>();
			return (it->second)->IsSystemPaused();
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			SystemType* SystemManager::GetSystem()
		{
			SystemMapIterator const it = FindSystem<SystemType>();
			return (std::static_pointer_cast<SystemType>(it->second)).get();
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			bool SystemManager::IsSystemPresent() const
		{
			return _systems.contains(GetSystemTypeID<SystemType>());
		}

		template <typename SystemType>
			requires IsValidSystemType<SystemType>
		SystemTypeID SystemManager::GetSystemTypeID() const
		{
			return DUtility::FamilyClassType<System>::GetTypeID<SystemType>();
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			SystemManager::SystemMapIterator SystemManager::FindSystem()
		{
			SystemMapIterator const it = _systems.find(GetSystemTypeID<SystemType>());
			if (it == _systems.end())
			{
				DFW_ERRORLOG("Unable to find System of T: {}", typeid(SystemType).name());
				DFW_ASSERT(false && "Unable to find System");
			}

			return it;
		}

#pragma endregion 

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
