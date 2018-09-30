#pragma once

#include <stdint.h>

namespace DECS {

	struct ComponentBitList {
		int64_t bit_list = 0;

		//template <class T>
		//bool ECSKeyLockSystem::IsComponentBitTrue() {
		//	if (component_bit_placement_.find(typeid(T) != component_bit_placement_.end())) {
		//		return true;
		//	} else {
		//		return false;
		//	}
		//}
	};

} // End of namespace ~ DECS