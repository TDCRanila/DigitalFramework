#pragma once

#include <Modules/Editor/EditorElement.h>

#include <Modules/Rendering/RenderTarget.h>

#include <imgui.h>

#include <bgfx/bgfx.h>

#include <string>

namespace DFW
{
    namespace DEditor
    {
        class GameViewport final : public EditorElement
        {
        public:
            GameViewport() = default;
            virtual ~GameViewport() = default;

            virtual void Init() override;
            virtual void Terminate() override;
            virtual void Display() override;

        private:
            SharedPtr<DRender::RenderTarget const> _viewport_render_target;
            ImVec2 _viewport_size;
            bool _is_viewport_window_open;

        };

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
