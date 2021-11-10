#pragma once

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Logging/Logger.h>

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
		public:
			~SystemManager() = default;

			template <typename SystemType, typename ...TArgs>
			requires IsValidSystemType<SystemType>
				void AddSystem(TArgs&&... a_args);

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				void RemoveSystem();

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				bool EnableSystem();

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				bool DisableSystem();

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				bool IsSystemDisabled() const;

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				SystemType* GetSystem();

			template <typename SystemType>
			requires IsValidSystemType<SystemType>
				bool IsSystemPresent();

			// TODO Implement System Dependencies / Priority

		protected:
			friend ECSModule;

			void Init();
			void Terminate();
			void UpdateSystems(Universe* const a_universe);

			SystemManager();

		private:
			// TODO Potentially better to use integer values instead of type_index for possibily 
			// a slight performance boost, but that depends on the comparisson implementation of 
			// typeinfo/type_index.
			std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;

		};

#pragma region Template Function Implementation

		template <typename SystemType, typename ...TArgs>
		requires IsValidSystemType<SystemType>
			void SystemManager::AddSystem(TArgs&&... a_args)
		{
			auto& type = typeid(SystemType);
			if (_systems.contains(type))
			{
				DFW_WARNLOG("Trying to add a new System that is already registered in the manager.");
			}
			else
			{
				std::shared_ptr<SystemType> system_ptr = std::make_shared<SystemType>(std::forward<TArgs>(a_args)...);

				system_ptr->_name = type.name();
				system_ptr->_id = DFW::GenerateDUID();
				system_ptr->_entity_manager = CoreService::GetECS()->EntityManager();

				_systems[type] = system_ptr;

				system_ptr->InternalInit();
			}
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			void SystemManager::RemoveSystem()
		{
			auto const it = _systems.find(typeid(SystemType));
			if (it == _systems.end())
			{
				DFW_WARNLOG("Trying to remove an System that is not registered in the manager.");
			}
			else
			{
				auto& [system_type, system_ptr] = (*it);
				system_ptr->InternalTerminate();
				_systems.erase(it);
			}
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			bool SystemManager::EnableSystem()
		{
			if (SystemType* system_ptr = GetSystem<SystemType>())
			{
				system_ptr->InternalPauseSystem(false);
				return true;
			}
			else
			{
				return false;
			}
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			bool SystemManager::DisableSystem()
		{
			if (SystemType* system_ptr = GetSystem<SystemType>())
			{
				system_ptr->InternalPauseSystem(true);
				return true;
			}
			else
			{
				return false;
			}
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			bool SystemManager::IsSystemDisabled() const
		{
			if (SystemType const* const system_ptr = const_cast<SystemManager*>(this)->GetSystem<SystemType>())
				return system_ptr->IsSystemPaused();
			else
				return false;
		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			SystemType* SystemManager::GetSystem()
		{
			auto& type = typeid(SystemType);
			auto it = _systems.find(type);
			if (it == _systems.end())
			{
				DFW_ERRORLOG("Unable to find System of T: {}", type.name());
				return {};
			}
			else
			{
				// TODO Research some more if this unique_ptr usage is fine, it is not
				// that we are moving the ownership or anything.
				return static_cast<SystemType*>((*it).second.get());
			}

		}

		template <typename SystemType>
		requires IsValidSystemType<SystemType>
			bool SystemManager::IsSystemPresent()
		{
			auto const& type = typeid(SystemType);
			if (!_systems.contains(type))
			{
				DFW_ERRORLOG("Unable to find System of T: {}", type.name());
				return false;
			}
			else
			{
				return true;
			}
		}

#pragma endregion 

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
