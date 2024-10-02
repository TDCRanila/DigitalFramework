#include <DFW/Modules/ECS/System.h>

#include <DFW/Modules/ECS/Managers/SystemManager.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace DFW
{
	namespace DECS
	{
		System::System()
			: _system_manager(nullptr)
			, _event_handler(nullptr)
			, _id(DFW_INVALID_DUID)
			, _type_id(DFW_INVALID_SYSTEM_TYPE_ID)
			, _name("Default System Name.")
			, _paused(false)
		{
		}

		void System::ExecuteBefore(System& a_system)
		{
			_system_manager->AddSystemDependency(a_system, *this);
		}

		void System::ExecuteAfter(System& a_system)
		{
			_system_manager->AddSystemDependency(*this, a_system);
		}

		void System::RemoveDependencies(System& a_system)
		{
			_system_manager->RemoveSystemDependency(*this, a_system);
			_system_manager->RemoveSystemDependency(a_system, *this);
		}

		void System::Init(EntityRegistry& /*a_registry*/)
		{
		}

		void System::Terminate(EntityRegistry& /*a_registry*/)
		{
		}

		void System::PreUpdate(EntityRegistry& /*a_registry*/)
		{
		}

		void System::Update(EntityRegistry& /*a_registry*/)
		{
		}

		void System::PostUpdate(EntityRegistry& /*a_registry*/)
		{
		}

		void System::UpdateSystemImGui(EntityRegistry& /*a_registry*/)
		{
		}

		void System::InternalInit(EntityRegistry& a_registry)
		{
			DFW_INFOLOG("Initialization System: {} - {}", _id, _name);

			this->Init(a_registry);
		}

		void System::InternalTerminate(EntityRegistry& a_registry)
		{
			DFW_INFOLOG("Terminating System: {} - {}", _id, _name);

			this->Terminate(a_registry);
		}

		void System::InternalPreUpdate(EntityRegistry& a_registry)
		{
			DFW_LOG("PreUpdate System: {} - {}", _id, _name);

			this->PreUpdate(a_registry);
		}

		void System::InternalUpdate(EntityRegistry& a_registry)
		{
			DFW_LOG("Update System: {} - {}", _id, _name);

			this->Update(a_registry);
		}

		void System::InternalPostUpdate(EntityRegistry& a_registry)
		{
			DFW_LOG("PostUpdate System: {} - {}", _id, _name);

			this->PostUpdate(a_registry);
		}

		void System::InternalPauseSystem(bool a_pause_on_true)
		{
			this->_paused = a_pause_on_true;
		}

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
