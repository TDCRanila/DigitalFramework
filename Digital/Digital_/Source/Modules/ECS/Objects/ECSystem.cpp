#include <Modules/ECS/Objects/ECSystem.h>

#include <Modules/ECS/Utility/ECSIDManager.h>

namespace DECS
{

	ECSystem::ECSystem(Key) :
		system_id_(ECSIDManager::GetNextSystemID()),
		entity_manager_(nullptr),
		system_name_("Base System Name.")
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
		return paused_;
	}

	void ECSystem::InternalInit(ECSEntityManager* a_entity_manager)
	{
		INFOLOG("Initialization System: " << system_id_ << " - " << system_name_);

		if (!a_entity_manager)
		{
			ERRORLOG("Passing an invalid Entity Manager to ECSystem.");
			DASSERT(false);
		}
		else
		{
			entity_manager_ = a_entity_manager;
		}

		this->Init(entity_manager_);
	}

	void ECSystem::InternalTerminate()
	{
		INFOLOG("Terminating System: " << system_id_ << " - " << system_name_);
		this->Terminate(entity_manager_);
	}

	void ECSystem::InternalPreUpdate()
	{
		INFOLOG("PreUpdate System: " << system_id_ << " - " << system_name_);
		this->PreUpdate(entity_manager_);
	}

	void ECSystem::InternalUpdate()
	{
		INFOLOG("Update System: " << system_id_ << " - " << system_name_);
		// TODO.
		this->Update(entity_manager_, float32(0.0f));
	}

	void ECSystem::InternalPostUpdate()
	{
		INFOLOG("PostUpdate System: " << system_id_ << " - " << system_name_);
		this->PostUpdate(entity_manager_);
	}

	void ECSystem::InternalPauseSystem(bool a_pause_on_true)
	{
		this->paused_ = a_pause_on_true;
	}

	SystemID ECSystem::GetID() const 
	{
		return system_id_;
	}

} // End of namespace ~ DECS
