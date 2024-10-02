#pragma once

#include <CoreSystems/Memory.h>

#include <string>

namespace DFW
{
    namespace DEditor
    {
        // FW Declare.
        class EditorElementContainer;

        using EditorElementNameID = std::string;

        class EditorElement
        {
        private:
            friend EditorElementContainer;

        public:
            EditorElement();
            virtual ~EditorElement();

            virtual void Init();
            virtual void Terminate();

            virtual void Update();
            virtual void Display();

            EditorElementNameID GetNameID() const;
            bool IsVisible() const;

            UniquePtr<EditorElementContainer> element_container;

        private:
            EditorElementNameID _element_name_id;
            bool _is_visible;

        };

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
