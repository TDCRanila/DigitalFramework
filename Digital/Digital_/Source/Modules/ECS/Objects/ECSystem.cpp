#include <Modules/ECS/Objects/ECSystem.h>

#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
	namespace DECS
	{
		System::System()
			: _system_manager(nullptr)
			, _entity_manager(nullptr)
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

		void System::PreUpdate(Universe& /*a_universe*/)
		{
		}

		void System::Update(Universe& /*a_universe*/)
		{
		}

		void System::PostUpdate(Universe& /*a_universe*/)
		{
		}

		void System::UpdateSystemImGui(Universe& /*a_universe*/)
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

		void System::InternalPreUpdate(Universe& a_universe)
		{
			DFW_LOG("PreUpdate System: {} - {}", _id, _name);

			this->PreUpdate(a_universe);
		}

		void System::InternalUpdate(Universe& a_universe)
		{
			DFW_LOG("Update System: {} - {}", _id, _name);

			this->Update(a_universe);
		}

		void System::InternalPostUpdate(Universe& a_universe)
		{
			DFW_LOG("PostUpdate System: {} - {}", _id, _name);

			this->PostUpdate(a_universe);
		}

		void System::InternalPauseSystem(bool a_pause_on_true)
		{
			this->_paused = a_pause_on_true;
		}

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
