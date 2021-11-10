#include <Modules/ECS/Objects/ECSystem.h>

#include <CoreSystems/Logging/Logger.h>

namespace ECS
{
	System::System(Key) 
		: _id(DCore::DFW_INVALID_DUID)
		, _entity_manager(nullptr)
		, _name("Default System Name.")
		, _paused(false)
	{ 
	}

	void System::Init() 
	{
	}

	void System::Terminate()
	{
	}
	
	void System::PreUpdate(Universe* const /*a_universe*/) 
	{
	}

	void System::Update(Universe* const /*a_universe*/) 
	{
	}

	void System::PostUpdate(Universe* const /*a_universe*/) 
	{
	}

	bool System::IsSystemPaused() const
	{
		return _paused;
	}

	DCore::DUID System::GetID() const
	{
		return _id;
	}

	std::string System::GetName() const
	{
		return _name;
	}

	EntityManager* const System::EntityManager() const
	{
		return _entity_manager;
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

	void System::InternalPreUpdate(Universe* const a_universe)
	{
		DFW_LOG("PreUpdate System: {} - {}", _id, _name);

		this->PreUpdate(a_universe);
	}

	void System::InternalUpdate(Universe* const a_universe)
	{
		DFW_LOG("Update System: {} - {}", _id, _name);
		
		this->Update(a_universe);
	}

	void System::InternalPostUpdate(Universe* const a_universe)
	{
		DFW_LOG("PostUpdate System: {} - {}", _id, _name);

		this->PostUpdate(a_universe);
	}

	void System::InternalPauseSystem(bool a_pause_on_true)
	{
		this->_paused = a_pause_on_true;
	}

} // End of namespace ~ ECS
