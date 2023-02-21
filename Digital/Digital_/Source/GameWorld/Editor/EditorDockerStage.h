#pragma once

#include <CoreSystems/Stage/Stage.h>

namespace DFW
{
	namespace DEditor
	{
		class EditorDocker final : public DFW::StageBase
		{
		public:
			EditorDocker(std::string const& a_stage_name, bool a_start_disabled);
			~EditorDocker() = default;

		private:
			virtual void OnUpdate() override;
			virtual void OnRenderImGui() override;

			virtual void OnAttached() override;
			virtual void OnRemoved() override;

			void SetupEditorMenubar();
			void SetupDockingSpace();

			bool _has_dockspace_been_created;

		};

	} // End of namespace ~ DFW

} // End of namespace ~ DFW.
