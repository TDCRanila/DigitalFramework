#pragma once

#include <CoreSystems/Stage/Stage.h>

namespace DFW
{
	namespace DEditor
	{
		class MainMenuBar : public DFW::StageBase
		{
		public:
			MainMenuBar(std::string const& a_stage_name, bool a_start_disabled);
			~MainMenuBar() = default;

		private:
			virtual void Update() override;
			virtual void RenderImGui() override;

			void Debug_ShowImGuiShowcase();

			bool _show_imgui_showcase;

		};

	} // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
