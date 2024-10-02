#pragma once

#include <DFW/Modules/Editor/EditorElement.h>

namespace DFW
{
    namespace DRender
    {
        struct RenderTarget;
        struct ViewTarget;
    }

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

        public:
            SharedPtr<DRender::RenderTarget const> GetViewportRenderTarget() const;

        private:
            SharedPtr<DRender::RenderTarget const> _viewport_render_target;
            SharedPtr<DRender::ViewTarget const> _viewport_view_target;

            bool _is_viewport_window_open;

        };

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
