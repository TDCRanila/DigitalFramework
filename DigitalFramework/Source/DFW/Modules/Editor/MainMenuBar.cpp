#include <DFW/Modules/Editor/MainMenuBar.h>

#include <DFW/CoreSystems/CoreServices.h>
#include <DFW/CoreSystems/Events/EventDispatcher.h>
#include <DFW/CoreSystems/ApplicationEvents.h>

#include <imgui/imgui.h>

namespace DFW
{
	namespace DEditor
	{
		MainMenuBar::MainMenuBar(std::string const& a_stage_name, bool a_start_disabled)
			: DFW::StageBase(a_stage_name, a_start_disabled)
			, _show_imgui_showcase(false)
		{
		}

		//MainMenuBar::~MainMenuBar()= default;

		void MainMenuBar::OnUpdate()
		{
		}

		void MainMenuBar::OnRenderImGui()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Explore"))
				{
					if (ImGui::MenuItem("Reset", nullptr))
						DFW::CoreService::GetAppEventHandler()->Broadcast<ApplicationResetEvent>();

					if (ImGui::MenuItem("Exit", nullptr))
						DFW::CoreService::GetAppEventHandler()->Broadcast<ApplicationCloseEvent>();

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Options"))
				{
					ImGui::MenuItem("Show ImGui Showcase", nullptr, &_show_imgui_showcase);

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}

			// Debug.
			Debug_ShowImGuiShowcase();

		}
		
		void MainMenuBar::Debug_ShowImGuiShowcase()
		{
			if (_show_imgui_showcase)
				ImGui::ShowDemoWindow(&_show_imgui_showcase);
		}

	} // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
