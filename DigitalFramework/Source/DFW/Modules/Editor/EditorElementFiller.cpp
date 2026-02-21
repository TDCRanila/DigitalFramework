#include <DFW/Modules/Editor/EditorElementFiller.h>

#include <imgui/imgui.h>

namespace DFW
{
    namespace DEditor
    {
        void EditorElementFiller::Display()
        {
            ImGui::Begin(GetNameID().c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::Text("This is a filler window - %s", GetNameID().c_str());
            static float32 float_input;
            ImGui::InputFloat("test-float", &float_input);
            static int32 int_input;
            ImGui::SliderInt("test-slider", &int_input, 0, 10);
            ImGui::End();
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
