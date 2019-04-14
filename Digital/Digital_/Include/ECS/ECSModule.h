#pragma once

#include <memory>

namespace DECS 
{

	// FW Declares.
	class ECSystemManager;
	class ECSEntityManager;
	class ECSComponentManager;

	class ECSModule final 
	{
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
		bool initialized_;

		std::unique_ptr<ECSystemManager>		system_manager_;
		std::unique_ptr<ECSEntityManager>		entity_manager_;
		std::unique_ptr<ECSComponentManager>	component_manager_;
	};

} // End of namespace ~ DECS
