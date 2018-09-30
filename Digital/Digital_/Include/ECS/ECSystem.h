#pragma once

#include <memory>
#include <unordered_map>

namespace DECS {

	// FW Declares
	class ECSEntity;
	class ECSManager;
	class ECSKeyLockSystem;
	class ECSIDManager;

	class ECSystem {
	public:
		ECSystem();
		~ECSystem();

		// init
		void Init();

		// update
		void Update();

		// terminate
		void Terminate();

		std::weak_ptr<ECSEntity> CreateEntity();

		// vector/map of normal systems
		
		// map of entities
		std::unordered_map<size_t, std::shared_ptr<ECSEntity>> entity_map;

		ECSKeyLockSystem* keylock_system_;
		ECSIDManager* id_manager_;



	};

} // End of namespace ~ DECS