#include <ECS/Objects/ECSystem.h>

#include <ECS/Utility/ECSIDManager.h>

namespace DECS
{

	ECSystem::ECSystem(Key) :
		system_id_(ECSIDManager::GetNextSystemID())
	{ /*EMPTY*/ }

	ECSystem::~ECSystem() { /*EMPTY*/ }

	void ECSystem::Init() { /*EMPTY*/ }

	void ECSystem::Terminate() { /*EMPTY*/ }

	void ECSystem::PreUpdate() { /*EMPTY*/ }

	void ECSystem::Update() { /*EMPTY*/ }

	void ECSystem::PostUpdate() { /*EMPTY*/ }

	SystemID ECSystem::GetID() const 
	{
		return system_id_;
	}

} // End of namespace ~ DECS
