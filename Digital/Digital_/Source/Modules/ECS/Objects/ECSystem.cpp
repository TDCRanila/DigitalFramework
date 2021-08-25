#include <Modules/ECS/Objects/ECSystem.h>

#include <Modules/ECS/Utility/ECSIDManager.h>

#include <CoreSystems/Logging/Logger.h>

namespace DECS
{

	ECSystem::ECSystem(Key) 
		: _system_id(-1) // TODO Proper Unitialized value.
		, _entity_manager(nullptr)
		, _system_name("Default System Name.")
		, _paused(false)
	{ 
		/*EMPTY*/
	}

	ECSystem::~ECSystem() { /*EMPTY*/ }

	void ECSystem::Init(ECSEntityManager* /*a_entity_manager*/) { }

	void ECSystem::Terminate(ECSEntityManager* /*a_entity_manager*/) { /*EMPTY*/ }

	void ECSystem::PreUpdate(ECSEntityManager* /*a_entity_manager*/) { /*EMPTY*/ }

	void ECSystem::Update(ECSEntityManager* /*a_entity_manager*/, float32 /*a_delta_time*/) { /*EMPTY*/ }

	void ECSystem::PostUpdate(ECSEntityManager* /*a_entity_manager*/) { /*EMPTY*/ }

	bool ECSystem::IsSystemPaused()
	{
		return _paused;
	}

	void ECSystem::InternalInit(ECSEntityManager* a_entity_manager)
	{
		_system_id = ECSIDManager::GetNextSystemID();

		DFW_INFOLOG("Initialization System: {} - {}", _system_id, _system_name);

		if (!a_entity_manager)
		{
			DFW_ERRORLOG("Passing an invalid Entity Manager to ECSystem.");
			DFW_ASSERT(false);
		}
		else
		{
			_entity_manager = a_entity_manager;
		}

		this->Init(_entity_manager);
	}

	void ECSystem::InternalTerminate()
	{
		DFW_INFOLOG("Terminating System: {} - {}", _system_id, _system_name);
		this->Terminate(_entity_manager);
	}

	void ECSystem::InternalPreUpdate()
	{
		DFW_LOG("PreUpdate System: {} - {}", _system_id, _system_name);
		this->PreUpdate(_entity_manager);
	}

	void ECSystem::InternalUpdate()
	{
		DFW_LOG("Update System: {} - {}", _system_id, _system_name);
		// TODO.
		this->Update(_entity_manager, float32(0.0f));
	}

	void ECSystem::InternalPostUpdate()
	{
		DFW_LOG("PostUpdate System: {} - {}", _system_id, _system_name);
		this->PostUpdate(_entity_manager);
	}

	void ECSystem::InternalPauseSystem(bool a_pause_on_true)
	{
		this->_paused = a_pause_on_true;
	}

	SystemID ECSystem::GetID() const 
	{
		return _system_id;
	}

} // End of namespace ~ DECS
