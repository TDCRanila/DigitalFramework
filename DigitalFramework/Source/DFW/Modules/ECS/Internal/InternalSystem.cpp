#include <DFW/Modules/ECS/System.h>

#include <DFW/Modules/ECS/Managers/SystemManager.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace DFW
{
	namespace DECS
	{
		InternalSystem::InternalSystem()
			: _system_manager(nullptr)
			, _event_handler(nullptr)
			, _id(DFW_INVALID_DUID)
			, _type_id(DFW_INVALID_SYSTEM_TYPE_ID)
			, _name("Default System Name.")
			, _paused(false)
		{
		}

		void InternalSystem::InternalInit(EntityRegistry& a_registry)
		{
			DFW_INFOLOG("Initialization System: {} - {}", _id, _name);

			this->Init(a_registry);
		}

		void InternalSystem::InternalTerminate(EntityRegistry& a_registry)
		{
			DFW_INFOLOG("Terminating System: {} - {}", _id, _name);

			this->Terminate(a_registry);
		}

		void InternalSystem::InternalPreUpdate(EntityRegistry& a_registry)
		{
			DFW_LOG("PreUpdate System: {} - {}", _id, _name);

			this->PreUpdate(a_registry);
		}

		void InternalSystem::InternalUpdate(EntityRegistry& a_registry)
		{
			DFW_LOG("Update System: {} - {}", _id, _name);

			this->Update(a_registry);
		}

		void InternalSystem::InternalPostUpdate(EntityRegistry& a_registry)
		{
			DFW_LOG("PostUpdate System: {} - {}", _id, _name);

			this->PostUpdate(a_registry);
		}

		void InternalSystem::InternalPauseSystem(bool a_pause_on_true)
		{
			this->_paused = a_pause_on_true;
		}

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
