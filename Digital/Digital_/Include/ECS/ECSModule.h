#pragma once

#include <ECS/Managers/ECSystemManager.h>
#include <ECS/Managers/ECSEntityManager.h>
#include <ECS/Managers/ECSComponentManager.h>
#include <ECS/Utility/ECSIDManager.h>

namespace DECS {

	class ECSModule final {
	public:
		ECSModule();
		~ECSModule();
			   
		void InitECS();

		void TerminateECS();

		void UpdateECS();

	protected:
		// friend	world module

		// add		system
		// remove	system
		// get		system

		// create	entity
		// delete	entity

		// create	component
		// delete	component
		// get		component

	private:
		//ECSystemManager	system_manager_;
		//ECSEntityManager	entity_manager_;
		//ECSComponentManager component_manager_;
	};

} // End of namespace ~ DECS
