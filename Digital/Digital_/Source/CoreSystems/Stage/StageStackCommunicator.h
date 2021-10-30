#pragma once

#include <CoreSystems/Stage/StageEvents.h>

namespace DCore
{
	// FW Declare.
	class StageStackController;

	class StageStackCommunicator
	{
	public:
		StageStackCommunicator() = default;
		~StageStackCommunicator() = default;

		void OnStageEventReceived(StageEvent& a_event);

	protected:
		friend class ApplicationInstance;

		void SetStageStackController(const StageStackController& a_controller);

	private:
		const StageStackController* _stage_stack_controller;

	};

} // End of namespace ~ DCore.
