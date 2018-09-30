#include <ECS/ECSEntity.h>

#include <ECS/ECSKeyLockSystem.h>
#include <ECS/ECSystem.h>

namespace DECS {

	ECSEntity::ECSEntity() { /*EMPTY*/ }

	ECSEntity::ECSEntity(ECSystem* a_system) : 
		parent_sys(a_system) { 
		/*EMPTY*/ 
	}

	ECSEntity::~ECSEntity() { /*EMPTY*/ }

} // End of namespace ~ DECS