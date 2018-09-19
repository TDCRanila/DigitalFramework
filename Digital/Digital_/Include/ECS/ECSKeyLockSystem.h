#pragma once

#include <AutoFactory/AutoFactory.h>

#include <unordered_map>

namespace DECS {

	class ECSKeyLockSystem {
	public:
		ECSKeyLockSystem();
		~ECSKeyLockSystem();

		template <class T>
		int64_t GetComponentBitPlacement();

		void GenerateComponentKeys();

	private:
		std::unordered_map<std::type_index, int64_t> component_bit_placement_;

	};

#pragma region Template Function Implementation

	template <class T>
	int64_t ECSKeyLockSystem::GetComponentBitPlacement() {
		// TODO: DEBUG->Safety Check
		return component_bit_placement_.find(typeid(T))->second;
	}

#pragma endregion

} // End of namespace ~ DECS