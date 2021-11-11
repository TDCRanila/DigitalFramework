#pragma once

#include <CoreSystems/ApplicationEvents.h>
#include <CoreSystems/Stage/StageEvents.h>
#include <CoreSystems/Stage/StageID.h>

namespace DFW
{
	// FW Declare.
	class ApplicationInstance;
	class StageStackController;
	class StageStackCommunicator;

	class StageBase
	{
	public:
		StageBase(std::string const& a_stage_name, bool a_start_disabled);
		virtual ~StageBase() = default;
		
		virtual void Update()		= 0;
		virtual void RenderImGui();

		virtual void OnAttached()	= 0;
		virtual void OnRemoved()	= 0;
				
		bool IsDisabled() const;
		virtual void OnEnable();
		virtual void OnDisable();

		virtual void OnApplicationEvent(ApplicationEvent const& a_event);
		virtual void OnStageEvent(StageEvent const& a_event);
 
		StageID GetID() const;
		std::string GetName() const;

		bool operator==(StageBase const& a_other);
	
	protected:
		void RequestEventBroadcast(StageEvent& a_event);
		std::shared_ptr<StageStackCommunicator> GetStageStackCommunicator() const;
	
	protected:
		friend StageStackController;

		void SetStageStackCommunicator(std::shared_ptr<StageStackCommunicator> const& a_communicator);
		void BindStageEventFunc(StageEventCallbackFunc const& a_event_callback_func);

	protected:
		friend ApplicationInstance;

		void Enable();
		void Disable();

	private:
		StageID _id;
		std::string _name; // TODO DEBUG IFElse

		std::shared_ptr<StageStackCommunicator> _stage_stack_communicator;
		StageEventCallbackFunc _stage_event_callback_func;

		bool _is_disabled;
		
	};

} // End of namespace ~ DFW.
