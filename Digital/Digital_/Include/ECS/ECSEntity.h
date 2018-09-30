#pragma once

#include <ECS/ECSystem.h>
#include <ECS/ECSComponent.h>
#include <ECS/ECSKeyLockSystem.h>

#include <string>
#include <memory>
#include <unordered_map>
#include <typeindex>

namespace DECS {

	class ECSEntity {
		public:
			ECSEntity();
			ECSEntity(ECSystem* a_system);
			~ECSEntity();
			
			ECSEntity(ECSEntity& a_entity) = delete;
			ECSEntity* operator=(const ECSEntity& a_eneity) = delete;


			// comp new.
			template <class T>
			std::weak_ptr<T> NewComponent();

			// comp getter.
			template <class T>
			std::weak_ptr<T> GetComponent();

			// comp del
			template <class T>
			bool DeleteComponent();

			std::unordered_map<std::type_index, std::shared_ptr<ECSComponent>> component_map_;

			ComponentBitList comp_bit_list;
			ECSystem* parent_sys;

			// entity id
			// name
			// tags

	};

#pragma region Template Function Implementation

	template <class T>
	std::weak_ptr<T> ECSEntity::NewComponent() {
		// Check if component already has been added.
		std::weak_ptr<T> c = GetComponent<T>();
		if (!c.expired()) {
			return c;
		} else {
			// Create component
			std::shared_ptr<ECSComponent> new_c = std::make_shared<T>();
			component_map_.emplace(typeid(T), new_c);

			// Update Entity Key
			parent_sys->keylock_system_->SetComponentBits<T>(&comp_bit_list);
			return std::static_pointer_cast<T>(new_c);
		}
	}

	template <class T>
	std::weak_ptr<T> ECSEntity::GetComponent() {
		auto it = component_map_.find(typeid(T));
		if (it != component_map_.end()) {
			return std::static_pointer_cast<T>(it->second);
		} else {
			return std::weak_ptr<T>();
		}
	}

	template <class T>
	bool ECSEntity::DeleteComponent() {
		auto it = component_map_.find(typeid(T));
		if (it != component_map_.end()) {
			//delete it->second.get();
			it->second.reset();
			component_map_.erase(it);

			// Update Entity Key
			parent_sys->keylock_system_->ResetComponentBits<T>(&comp_bit_list);
			return true;
		} else {
			return false;
		}
	}

#pragma endregion

} // End of namespace ~ DECS