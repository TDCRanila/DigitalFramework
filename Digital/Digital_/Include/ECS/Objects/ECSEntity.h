#pragma once

#include <ECS/Utility/ECSIDManager.h>
#include <ECS/Utility/ECSCompBitList.h>

namespace DECS 
{

	class ECSEntity final 
	{
	public:
		ECSEntity();
		~ECSEntity();

		EntityID GetID() const;
		
		bool PendingDelete() const;

		ComponentBitList component_bit_list;

	private:
		EntityID entity_id_;
		bool pending_delete_;
	};

} // End of namespace ~ DECS
