#pragma once

#include <DFW/CoreSystems/Stage/StageID.h>

namespace DFW
{
	// FW Declare.
	class StageStackController;

	class StageBase
	{
		friend StageStackController;

	public:
		StageBase(std::string const& a_stage_name, bool a_start_disabled);
		virtual ~StageBase() = default;
		
		bool operator==(StageBase const& a_other);

		void Enable();
		void Disable();
		bool IsDisabled() const;

		StageID GetID() const;
		std::string GetName() const;

	protected:
		virtual void OnUpdate() = 0;
		virtual void OnRender();
		virtual void OnRenderImGui();

		virtual void OnAttached();
		virtual void OnRemoved();

		virtual void OnEnable();
		virtual void OnDisable();

	private:
		StageID _id;
		std::string _name;

		bool _is_disabled;
		
	};

} // End of namespace ~ DFW.
