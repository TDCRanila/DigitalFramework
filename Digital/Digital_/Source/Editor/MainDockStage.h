#pragma once

#include <CoreSystems/Stage/Stage.h>

namespace DEditor
{
	class DockerStage : public DCore::StageBase
	{
	public:
		DockerStage(std::string a_stage_name, bool a_start_disabled = false);
		~DockerStage() = default;

		virtual void RenderImGui() override;

		virtual void Update() override;

		virtual void OnAttached() override;

		virtual void OnRemoved() override;

	private:
		bool _first_time_start;
		bool _show_imgui_showcase;

	};

} // End of namespace ~ DCore
