#pragma once

#include <Editor/EditorElementContainer.h>
#include <Editor/Game/EditorGameViewport.h>

#include <CoreSystems/Stage/Stage.h>

#include <unordered_map>

namespace DFW
{
	namespace DEditor
	{
		class DockerStage final : public DFW::StageBase
		{
		public:
			DockerStage(std::string const& a_stage_name, bool a_start_disabled);
			~DockerStage() = default;

			EditorElementContainer element_container;

		private:
			virtual void Update() override;
			virtual void RenderImGui() override;

			virtual void OnAttached() override;
			virtual void OnRemoved() override;

			void SetupMenubar();
			void SetupDockingSpace();

			bool _has_dockspace_been_created;
			bool _show_imgui_showcase;

		};

	} // End of namespace ~ DFW

} // End of namespace ~ DFW.
