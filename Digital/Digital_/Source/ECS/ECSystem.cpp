#include <ECS/ECSystem.h>

#include <ECS/ECSKeyLockSystem.h>
#include <ECS/ECSIDManager.h>
#include <ECS/ECSEntity.h>

namespace DECS {

	ECSystem::ECSystem() :
		keylock_system_(nullptr), 
		id_manager_(nullptr)
	{
		keylock_system_ = new ECSKeyLockSystem();
		id_manager_		= new ECSIDManager();
	}

	ECSystem::~ECSystem() {
		// KeyLock
		delete keylock_system_;
		keylock_system_ = nullptr;

		// ID Manager
		delete id_manager_;
		id_manager_ = nullptr;

	}

	void ECSystem::Init() {
		// Generate KeyLock Component Keys
		keylock_system_->GenerateComponentKeys();
	}

	void ECSystem::Update() {}

	void ECSystem::Terminate() {}

	std::weak_ptr<ECSEntity> ECSystem::CreateEntity() {
		std::shared_ptr<ECSEntity> e = std::make_shared<ECSEntity>(this);

		entity_map.emplace(id_manager_->GetNextEntityID(), e);

		return e;
	}

} // End of namespace ~ DECS