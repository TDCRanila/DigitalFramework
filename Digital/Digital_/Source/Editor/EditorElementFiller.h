#pragma once

#include <Editor/EditorElement.h>

#include <string>

namespace DFW
{
    namespace DEditor
    {
        class EditorElementFiller : public EditorElement
        {
        public:
            EditorElementFiller() = default;
            virtual ~EditorElementFiller() = default;

        private:
            virtual void Display() override;

        };

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
