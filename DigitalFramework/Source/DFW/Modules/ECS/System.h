#pragma once

#include <DFW/Modules/ECS/Internal/InternalSystem.h>
#include <DFW/Modules/ECS/Managers/SystemManager.h>

#include <DFW/Modules/ECS/Utility/ECSTemplateUtility.h>

#include <DFW/Utility/AutoFactory/AutoFactory.h>

#include <string>

namespace DFW
{
	namespace DECS
	{
		class EntityRegistry;

		class System : public InternalSystem, public DFactory::AutoFactory<System>
		{
		public:
			System() = default;
			virtual ~System() = default;

		public:
			template <typename ReliedOnSystemType>
			requires IsValidSystemType<ReliedOnSystemType>
				void ExecuteBefore();

			template <typename ReliedOnSystemType>
			requires IsValidSystemType<ReliedOnSystemType>
				void ExecuteAfter();

			template <typename ReliedOnSystemType>
			requires IsValidSystemType<ReliedOnSystemType>
				void RemoveDependencies();

		protected:
			// Can be overwritten by derived class.
			virtual void Init(EntityRegistry& a_registry);
			virtual void Terminate(EntityRegistry& a_registry);

			virtual void PreUpdate(EntityRegistry& a_registry);
			virtual void Update(EntityRegistry& a_registry);
			virtual void PostUpdate(EntityRegistry& a_registry);

			virtual void UpdateSystemImGui(EntityRegistry& a_registry);

		};

#pragma region Template Function Implementation

		template <typename ReliedOnSystemType>
		requires IsValidSystemType<ReliedOnSystemType>
			void System::ExecuteBefore()
		{
			DFW::DECS::SystemManager& system_manager = SystemManager();
			system_manager.AddSystemDependency(system_manager.GetSystemTypeID<ReliedOnSystemType>(), GetTypeID());
		}

		template <typename ReliedOnSystemType>
		requires IsValidSystemType<ReliedOnSystemType>
			void System::ExecuteAfter()
		{
			DFW::DECS::SystemManager& system_manager = SystemManager();
			system_manager.AddSystemDependency(GetTypeID(), system_manager.GetSystemTypeID<ReliedOnSystemType>());
		}

		template <typename ReliedOnSystemType>
		requires IsValidSystemType<ReliedOnSystemType>
			void System::RemoveDependencies()
		{
			DFW::DECS::SystemManager& system_manager = SystemManager();
			system_manager.RemoveSystemDependency(GetTypeID(), system_manager.GetSystemTypeID<ReliedOnSystemType>());
			system_manager.RemoveSystemDependency(system_manager.GetSystemTypeID<ReliedOnSystemType>(), GetTypeID());
		}

#pragma region

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
