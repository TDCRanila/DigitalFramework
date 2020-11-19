#include <Zynthurism/Editor/EditorStage.h>

namespace DEditor
{

    void EditorStage::RenderImGui()
    {
        BeginEditorRender();

        // TODO
        // for loop editor elemtsn -> render update

        EndEditorRender();
    }

    void EditorStage::InitEditor()
    {

    }

    void EditorStage::BeginEditorRender()
    {
        // TODO Make them non-static variables.
        static bool opt_fullscreen = true;
        static bool opt_padding = false;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::SetNextWindowBgAlpha(1.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        if (!ImGui::Begin(_main_editor_id, &_hide_main_editor, window_flags))
        {
            return;
        }

        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // Dockspace MenuBar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Dockspace Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Enable Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Enable Padding", NULL, &opt_padding);

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    void EditorStage::EndEditorRender()
    {
        ImGui::End();
    }

} // End of namespace ~ DGame
