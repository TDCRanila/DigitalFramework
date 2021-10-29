#include <Modules/ECS/Objects/ECSystem.h>

#include <CoreSystems/Logging/Logger.h>

namespace DECS
{
	ECSystem::ECSystem(Key) 
		: _id(DCore::InvalidDUID())
		, _entity_manager(nullptr)
		, _name("Default System Name.")
		, _paused(false)
	{ 
	}

	void ECSystem::Init() 
	{
	}

	void ECSystem::Terminate()
	{
	}
	
	void ECSystem::PreUpdate(ECSUniverse* const /*a_universe*/) 
	{
	}

	void ECSystem::Update(ECSUniverse* const /*a_universe*/) 
	{
	}

	void ECSystem::PostUpdate(ECSUniverse* const /*a_universe*/) 
	{
	}

	bool ECSystem::IsSystemPaused() const
	{
		return _paused;
	}

	DCore::DUID ECSystem::GetID() const
	{
		return _id;
	}

	std::string ECSystem::GetName() const
	{
		return _name;
	}

	ECSEntityManager* const ECSystem::EntityManager() const
	{
		return _entity_manager;
	}

	void ECSystem::InternalInit()
	{
		DFW_INFOLOG("Initialization System: {} - {}", _id, _name);

		this->Init();
	}

	void ECSystem::InternalTerminate()
	{
		DFW_INFOLOG("Terminating System: {} - {}", _id, _name);

		this->Terminate();
	}

	void ECSystem::InternalPreUpdate(ECSUniverse* const a_universe)
	{
		DFW_LOG("PreUpdate System: {} - {}", _id, _name);

		this->PreUpdate(a_universe);
	}

	void ECSystem::InternalUpdate(ECSUniverse* const a_universe)
	{
		DFW_LOG("Update System: {} - {}", _id, _name);
		
		this->Update(a_universe);
	}

	void ECSystem::InternalPostUpdate(ECSUniverse* const a_universe)
	{
		DFW_LOG("PostUpdate System: {} - {}", _id, _name);

		this->PostUpdate(a_universe);
	}

	void ECSystem::InternalPauseSystem(bool a_pause_on_true)
	{
		this->_paused = a_pause_on_true;
	}

} // End of namespace ~ DECS
