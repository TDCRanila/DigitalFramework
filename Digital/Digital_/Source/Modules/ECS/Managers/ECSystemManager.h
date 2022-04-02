#pragma once

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Logging/Logger.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/ECS/Utility/ECSConcepts.h>

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace DFW
{
	namespace DECS
	{
		// FW Declare
		class ECSModule;
		class Universe;

		constexpr int64 DFW_SYSTEM_RESERVE_AMOUNT = 16;

		class SystemManager final
		{
		private:
			friend ECSModule;

		public:
			SystemManager();
			~SystemManager();

			template <typename SystemType, typename ...TArgs>
			requires IsValidSystemType<SystemType>
				void AddSystem(TArgs&&... a_args);

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				void RemoveSystem();

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
				bool IsSystemPresent();

			// TODO Implement System Dependencies / Priority.

		private:
			void Terminate();

			void UpdateSystems(Universe& a_universe);
			void UpdateSystemsImGui(Universe& a_universe);

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				std::unordered_map<std::type_index, SharedPtr<System>>::iterator FindSystem();

		private:
			std::unordered_map<std::type_index, SharedPtr<System>> _systems;
			using SystemMapIterator = std::unordered_map<std::type_index, SharedPtr<System>>::iterator;

		};

#pragma region Template Function Implementation

		template <typename SystemType, typename ...TArgs>
		requires IsValidSystemType<SystemType>
			void SystemManager::AddSystem(TArgs&&... a_args)
		{
			type_info const& type = typeid(SystemType);
			if (_systems.contains(type))
			{
				DFW_WARNLOG("Trying to add a new System that is already registered in the manager.");
				return;
			}
			
			SharedPtr<SystemType> system_ptr = MakeShared<SystemType>(std::forward<TArgs>(a_args)...);

			system_ptr->_name			= type.name();
			system_ptr->_id				= DFW::GenerateDUID();
			system_ptr->_system_manager	= this;
			system_ptr->_entity_manager	= &CoreService::GetECS()->EntityManager();
			system_ptr->_event_handler	= &CoreService::GetECS()->EventHandler();

			DFW_ASSERT(system_ptr->_entity_manager);
			DFW_ASSERT(system_ptr->_event_handler);

			_systems.emplace(type, system_ptr);

			system_ptr->InternalInit();
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			void SystemManager::RemoveSystem()
		{
			SystemMapIterator const& it = FindSystem<SystemType>();
			it->second->InternalTerminate();
			_systems.erase(it);
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			void SystemManager::EnableSystem()
		{
			SystemMapIterator const& it = FindSystem<SystemType>();
			return (it->second)->InternalPauseSystem(false);
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			void SystemManager::DisableSystem()
		{
			SystemMapIterator const& it = FindSystem<SystemType>();
			return (it->second)->InternalPauseSystem(true);
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			bool SystemManager::IsSystemDisabled() const
		{
			SystemMapIterator const& it = FindSystem<SystemType>();
			return (it->second)->IsSystemPaused();
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			SystemType* SystemManager::GetSystem()
		{
			SystemMapIterator const& it = FindSystem<SystemType>();
			return std::static_pointer_cast<SystemType>(it->second).get();
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			bool SystemManager::IsSystemPresent()
		{
			if (_systems.contains(typeid(SystemType)))
				return true;
			else
				return false;
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			SystemManager::SystemMapIterator SystemManager::FindSystem()
		{
			type_info const& type = typeid(SystemType);
			SystemManager::SystemMapIterator const& it = _systems.find(type);
			if (it == _systems.end())
			{
				DFW_ERRORLOG("Unable to find System of T: {}", type.name());
				DFW_ASSERT(false && "Unable to find System");
			}

			return it;
		}

#pragma endregion 

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
