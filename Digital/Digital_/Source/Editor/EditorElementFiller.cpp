#include <Editor/EditorElementFiller.h>

#include <imgui/imgui.h>

namespace DFW
{
    namespace DEditor
    {
        void EditorElementFiller::Display()
        {
            ImGui::Begin(GetNameID().c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::Text("This is a filler window - %s", GetNameID().c_str());
            ImGui::End();
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
