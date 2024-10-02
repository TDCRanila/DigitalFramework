#include <DFW/Modules/Editor/EditorElementContainer.h>

namespace DFW
{
    namespace DEditor
    {
        EditorElementContainer::EditorElementContainer()
            : _id(GenerateDUID())
        {
        }

        void EditorElementContainer::UpdateElements()
        {
            for (auto const& [element_name, element_ptr] : _element_container)
                    element_ptr->Update();
        }

        void EditorElementContainer::DisplayElements()
        {
            for (auto const& [element_name, element_ptr] : _element_container)
                if (element_ptr->_is_visible)
                    element_ptr->Display();
        }

        void EditorElementContainer::ReleaseEditorElements()
        {
            for (auto const& [name, editor_element] : _element_container)
                editor_element->Terminate();

            _element_container.clear();
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
