#pragma once

#include <stdint.h>

namespace DECS {

	class ECSComponent {
	public:
		ECSComponent() {}
		ECSComponent(int64_t a_component_id) : component_id_(a_component_id) { /*EMPTY*/ }
		~ECSComponent() {}

		// serialize

		// inspect

		// comp id
		int64_t component_id_;
		// name
	};

} // End of namespace ~ DECS
