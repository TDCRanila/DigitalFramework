#include <DFW/Modules/Editor/EditorElement.h>

#include <DFW/Modules/Editor/EditorElementContainer.h>

namespace DFW
{
    namespace DEditor
    {
        EditorElement::EditorElement()
            : element_container(MakeUnique<EditorElementContainer>())
            , _element_name_id("")
            , _is_visible(true)
        {
        }

        EditorElement::~EditorElement() = default;

        void EditorElement::Init()
        {
        }

        void EditorElement::Terminate()
        {
        }

        void EditorElement::Update()
        {
        }

        void EditorElement::Display()
        {
        }

        EditorElementNameID EditorElement::GetNameID() const
        {
            return _element_name_id;
        }

        bool EditorElement::IsVisible() const
        {
            return _is_visible;
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
