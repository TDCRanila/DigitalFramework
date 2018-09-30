#pragma once

#include <unordered_map>
#include <typeindex>
#include <stdint.h>

namespace DECS {

	struct ComponentBitList {
		int64_t bit_list = 0;
	};

	class ECSKeyLockSystem {
	public:
		ECSKeyLockSystem();
		~ECSKeyLockSystem();

		void GenerateComponentKeys();
		
		template <class T>
		bool IsComponentBitTrue(ComponentBitList* a_bit_var);

		template <class T>
		void SetComponentBits(ComponentBitList* a_bit_var);

		template <class T>
		void ResetComponentBits(ComponentBitList* a_bit_var);

		template <class T>
		int8_t GetComponentBitPlacement();

	private:
		std::unordered_map<std::type_index, int8_t> component_bit_placement_;

	};

#pragma region Template Function Implementation

	template <class T>
	bool ECSKeyLockSystem::IsComponentBitTrue(ComponentBitList* a_bit_var) {
		auto it = component_bit_placement_.find(typeid(T));
		if (it != component_bit_placement_.end()) {
			int64_t temp = 1;
			return a_bit_var & (temp << it->second);
		}
	}

	template <class T>
	void ECSKeyLockSystem::SetComponentBits(ComponentBitList* a_bit_var) {
		int64_t temp = 1;
		(a_bit_var->bit_list |= (temp << GetComponentBitPlacement<T>()));
	}

	template <class T>
	void ECSKeyLockSystem::ResetComponentBits(ComponentBitList* a_bit_var) {
		int64_t temp = 1;
		(a_bit_var->bit_list &= ~(temp << GetComponentBitPlacement<T>()));
	}

	template <class T>
	int8_t ECSKeyLockSystem::GetComponentBitPlacement() {
		auto it = component_bit_placement_.find(typeid(T));
		if (it != component_bit_placement_.end()) {
			return it->second;
		} else {
			return -1;
		}
		// TODO: DEBUG->Safety Check
	}

#pragma endregion

} // End of namespace ~ DECS