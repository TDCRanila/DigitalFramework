#pragma once

#include <Modules/Editor/EditorElement.h>

#include <Utility/TemplateUtility.h>

#include <CoreSystems/DUID.h>
#include <CoreSystems/Memory.h>
#include <CoreSystems/Logging/Logger.h>

#include <unordered_map>
#include <string>

namespace DFW
{
    namespace DEditor
    {
        template <typename EditorElementType>
        concept IsValidEditorElementType = IsDerivedFrom<EditorElementType, EditorElement>;
        
        using EditorElementIterator = std::unordered_map<std::string, UniquePtr<EditorElement>>::iterator;
        using EditorElementConstIterator = std::unordered_map<std::string, UniquePtr<EditorElement>>::const_iterator;
        using EditorElementContainerID = DFW::DUID;

        class EditorElementContainer final
        {
        public:
            EditorElementContainer();
            ~EditorElementContainer() = default;

            void UpdateElements();
            void DisplayElements();

            template <typename EditorElementType>
            EditorElementType& AddEditorElement(EditorElementNameID const& a_element_name_id);

            template <typename EditorElementType>
            void RemoveEditorElement(EditorElementNameID const& a_element_name_id);
            void ReleaseEditorElements();

            template <typename EditorElementType>
            void HideEditorElement(EditorElementNameID const& a_element_name_id);
            template <typename EditorElementType>
            void ShowEditorElement(EditorElementNameID const& a_element_name_id);

            EditorElementIterator begin() { return _element_container.begin(); }
            EditorElementIterator end() { return _element_container.end(); }

            EditorElementConstIterator begin() const { return _element_container.begin(); }
            EditorElementConstIterator end() const { return _element_container.end(); }

        private:
            std::unordered_map<EditorElementNameID, UniquePtr<EditorElement>> _element_container;
            EditorElementContainerID _id;
        };

#pragma region Template Function Implementation
        template <typename EditorElementType>
        EditorElementType& EditorElementContainer::AddEditorElement(EditorElementNameID const& a_element_name_id)
        {
            if constexpr (not IsValidEditorElementType<EditorElementType>)
            {
                static_assert(always_false<EditorElementType>::value, "Trying to add an EditorElement, but the type is invalid.");
                return nullptr;
            }

            auto const& [it, result] = _element_container.emplace(a_element_name_id, MakeUnique<EditorElementType>());
            UniquePtr<EditorElement> const& editor_element = (*it).second;
            editor_element->_element_name_id = a_element_name_id;
            EditorElement* const editor_element_ptr = editor_element.get();
            if (!result)
            {
                DFW_INFOLOG("EditorElement with NameID: {} - already present in container-{}.", a_element_name_id, _id);
                return (*static_cast<EditorElementType*>(editor_element_ptr));
            }
            else
            {
                editor_element->Init();
                return (*static_cast<EditorElementType*>(editor_element_ptr));
            }
        }

        template <typename EditorElementType>
        void EditorElementContainer::RemoveEditorElement(EditorElementNameID const& a_element_name_id)
        {
            if constexpr (not IsValidEditorElementType<EditorElementType>)
            {
                static_assert(always_false<EditorElementType>::value, "Trying to remove an EditorElement, but the type is invalid.");
            }

            if (auto const& it = _element_container.find(a_element_name_id); it != _element_container.end())
            {
                (*it).second->Terminate();
                _element_container.erase(it);
            }
        }

        template <typename EditorElementType>
        void EditorElementContainer::HideEditorElement(EditorElementNameID const& a_element_name_id)
        {
            if constexpr (not IsValidEditorElementType<EditorElementType>)
            {
                static_assert(always_false<EditorElementType>::value, "Trying to make an EditorElement hidden, but the type is invalid.");
            }

            if (auto const& it = _element_container.find(a_element_name_id); it != _element_container.end())
                (*it).second->_is_visible = false;
        }

        template <typename EditorElementType>
        void EditorElementContainer::ShowEditorElement(EditorElementNameID const& a_element_name_id)
        {
            if constexpr (not IsValidEditorElementType<EditorElementType>)
            {
                static_assert(always_false<EditorElementType>::value, "Trying to make an EditorElement visible, but the type is invalid.");
            }

            if (auto const& it = _element_container.find(a_element_name_id); it != _element_container.end())
                (*it).second->_is_visible = true;
        }

#pragma endregion 

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
