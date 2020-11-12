#pragma once

#include <CoreSystems/Events/EventImplementation.h>

#include <Defines/IDDefines.h>

// TODO remove
#include <imgui/imgui.h>

namespace DCore
{
	// FW Declare.
	class StageStackCommunicator;

	class StageBase
	{
	public:
		StageBase(std::string a_stage_name, bool a_start_disabled = false);
		virtual ~StageBase() = default;
		
		virtual void Update()		= 0;
		virtual void RenderImGui();

		virtual void OnAttached()	= 0;
		virtual void OnRemoved()	= 0;
				
		bool IsDisabled() const;
		virtual void OnEnable();
		virtual void OnDisable();

		virtual void OnApplicationEvent(ApplicationEvent& a_event);
 
		StageID GetID() const;
		std::string GetName() const;

		bool operator==(const StageBase& a_other) { return this->_id == a_other._id; }

	protected:
		friend class StageStackController;

		std::shared_ptr<StageStackCommunicator> _stage_stack_communicator;

	protected:

		friend class ApplicationInstance;

		void Enable();
		void Disable();

	private:

		StageID _id;

		// TODO DEBUG
		std::string _name;

		bool _is_disabled;

	};

	// TODO REMOVE
	class StageExample2
	{
	public:
		StageExample2() = default;
	};

	class StageExample : public StageBase
	{
	public:
		StageExample(std::string a_stage_name, bool a_start_disabled = false) 
			: StageBase(a_stage_name, a_start_disabled)
		{
		}

		~StageExample() = default;

		virtual void RenderImGui() override
		{
			ImGui::Begin(GetName().c_str());

			ImGui::Text("Hello World! uwu - %s", GetName().c_str());

			ImGui::End();
		}

		virtual void Update() override
		{

		}

		virtual void OnAttached() override
		{

		}

		virtual void OnRemoved() override
		{

		}
	};

} // End of namespace ~ DCore.
