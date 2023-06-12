#include <Modules/ECS/System.h>

#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
	namespace DECS
	{
		System::System()
			: _system_manager(nullptr)
			, _event_handler(nullptr)
			, _id(DFW::DFW_INVALID_DUID)
			, _name("Default System Name.")
			, _paused(false)
		{
		}

		DFW::DUID System::GetID() const
		{
			return _id;
		}

		std::string System::GetName() const
		{
			return _name;
		}

		void System::Init()
		{
		}

		void System::Terminate()
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

		bool System::IsSystemPaused() const
		{
			return _paused;
		}

		void System::InternalInit()
		{
			DFW_INFOLOG("Initialization System: {} - {}", _id, _name);

			this->Init();
		}

		void System::InternalTerminate()
		{
			DFW_INFOLOG("Terminating System: {} - {}", _id, _name);

			this->Terminate();
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
