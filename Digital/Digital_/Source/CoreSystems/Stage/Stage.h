#pragma once

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
		
		void Enable();
		void Disable();

		virtual void Update() = 0;
		virtual void RenderImGui();

		virtual void OnAttached();
		virtual void OnRemoved();
				
		bool IsDisabled() const;
		virtual void OnEnable();
		virtual void OnDisable();
 
		StageID GetID() const;
		std::string GetName() const;

		bool operator==(StageBase const& a_other);
		
	private:
		StageID _id;
		std::string _name;

		bool _is_disabled;
		
	};

} // End of namespace ~ DFW.
