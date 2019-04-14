#include <ECS/Objects/ECSEntity.h>

#include <ECS/Utility/ECSIDManager.h>

namespace DECS 
{

	ECSEntity::ECSEntity() :
		entity_id_(ECSIDManager::GetNextEntityID())
	{ /*EMPTY*/ }

	ECSEntity::~ECSEntity() { /*EMPTY*/ }

	EntityID ECSEntity::GetID() const 
	{
		return entity_id_;
	}

} // End of namespace ~ DECS
