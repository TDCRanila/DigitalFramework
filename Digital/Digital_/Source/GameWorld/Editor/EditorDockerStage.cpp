#include <GameWorld/Editor/EditorDockerStage.h>

#include <GameWorld/Editor/EditorSystem.h>

#include <CoreSystems/CoreServices.h>
#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/ECSystemManager.h>

#include <imgui/imgui_internal.h>

namespace DFW
{
    namespace DEditor
    {
        EditorDocker::EditorDocker(std::string const& a_stage_name, bool a_start_disabled)
            : DFW::StageBase(a_stage_name, a_start_disabled)
            , _has_dockspace_been_created(false)
        {
        }

        void EditorDocker::Update()
        {
        }

        void EditorDocker::RenderImGui()
        {
            DFW_ASSERT(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable);

            ImGuiViewport const* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

            ImGuiWindowFlags const dockspace_window_flags =
                ImGuiWindowFlags_MenuBar
                | ImGuiWindowFlags_NoDocking
                | ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoBringToFrontOnFocus
                | ImGuiWindowFlags_NoNavFocus
                | ImGuiWindowFlags_NoBackground;

            { // Dockspace Window.
                ImGui::Begin("Window-DockSpace", nullptr, dockspace_window_flags);
                ImGui::PopStyleVar();
                ImGui::PopStyleVar(2);

                SetupDockingSpace();

                ImGui::End();
            }
        }

        void EditorDocker::OnAttached()
        {
            CoreService::GetECS()->SystemManager().AddSystem<EditorSystem>();
        }

        void EditorDocker::OnRemoved()
        {
            CoreService::GetECS()->SystemManager().RemoveSystem<EditorSystem>();
        }

        void EditorDocker::SetupEditorMenubar()
        {
            // Editor Menubar
            if (ImGui::BeginMainMenuBar())
            {
                ImGui::Separator();

                ImGui::EndMainMenuBar();
            }
        }

        void EditorDocker::SetupDockingSpace()
        {
            // DockSpace
            ImGuiID dockspace_id = ImGui::GetID("Main-DockSpace");
            ImGuiDockNodeFlags const dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            if (!_has_dockspace_been_created)
            {
                _has_dockspace_been_created = true;

                ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

                // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
                //  window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
                // out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
                // splitting these nodes should be/are done in correct order
                ImGuiID const dock_id_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.05f, nullptr, &dockspace_id);
                ImGuiID const dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.15f, nullptr, &dockspace_id);
                ImGuiID const dock_id_bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
                ImGuiID const dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.15f, nullptr, &dockspace_id);

                ImGuiID const dock_id_up = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.05f, nullptr, &dockspace_id);
                ImGuiID const dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

                ImGuiID const dock_id_center = ImGui::DockBuilderGetCentralNode(dockspace_id)->ID;

                // we now dock our windows into the docking node we made above
                ImGui::DockBuilderDockWindow(/*"Top"*/"Toolbar", dock_id_top);
                ImGui::DockBuilderDockWindow(/*"Bottom"*/"Console", dock_id_bottom);

                ImGui::DockBuilderDockWindow("Left", dock_id_left);
                ImGui::DockBuilderDockWindow(/*"Right"*/"Viewer", dock_id_right);

                ImGui::DockBuilderDockWindow("Up", dock_id_up);
                ImGui::DockBuilderDockWindow("Down", dock_id_down);

                ImGui::DockBuilderDockWindow("Viewport", dock_id_center);

                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
