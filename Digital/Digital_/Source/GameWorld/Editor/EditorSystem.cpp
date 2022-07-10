#include <GameWorld/Editor/EditorSystem.h>

#include <GameWorld/Editor/GameViewport.h>

#include <Modules/Editor/EditorElementFiller.h>

#include <imgui/imgui_internal.h>

namespace DFW
{
    namespace DEditor
    {
        void EditorSystem::Update(DECS::Universe& a_universe)
        {
            element_container.UpdateElements();
        }

        void EditorSystem::UpdateSystemImGui(DECS::Universe& a_universe)
        {
            element_container.DisplayElements();

        }

        void EditorSystem::Init()
        {
            element_container.AddEditorElement<GameViewport>("Viewport");
            element_container.AddEditorElement<EditorElementFiller>("Toolbar");
            element_container.AddEditorElement<EditorElementFiller>("Viewer");
        }

        void EditorSystem::Terminate()
        {
            element_container.ReleaseEditorElements();
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
