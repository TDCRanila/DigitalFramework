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

	//TODO Make them protected again - Was testing out things: - protected:
		// friend	world module

		// add		system
		// remove	system
		// get		system

		// create	entity
		// delete	entity

		// create	component
		// delete	component
		// get		component

	//TODO Make them private again - Was testing out things - private:
		bool initialized_;

		ECSystemManager*		system_manager_;
		ECSEntityManager*		entity_manager_;
		ECSComponentManager*	component_manager_;
	};

} // End of namespace ~ DECS
