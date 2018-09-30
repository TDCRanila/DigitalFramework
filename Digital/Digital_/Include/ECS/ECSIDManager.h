#pragma once

#include <stdint.h>

namespace DECS {

	class ECSIDManager {
	public:
		ECSIDManager() :
			next_system_id_(0), 
			next_entity_id_(0), 
			next_component_id_(0)
		{ /*EMPTY*/ }

		~ECSIDManager() { /*EMPTY*/ }

		inline void ResetIDCounters() {
			next_system_id_		= 0;
			next_entity_id_		= 0;
			next_component_id_	= 0;
		}

		inline int64_t GetNextSystemID()	{ return next_system_id_++;		}
		inline int64_t GetNextEntityID()	{ return next_entity_id_++;		}
		inline int64_t GetNextComponentID() { return next_component_id_++;	}

	private:
		int64_t next_system_id_;
		int64_t next_entity_id_;
		int64_t next_component_id_;

	};

} // End of namespace ~ DECS