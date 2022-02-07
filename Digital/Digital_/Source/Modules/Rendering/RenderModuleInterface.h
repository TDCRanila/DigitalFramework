#pragma once

#include <CoreSystems/ApplicationEvents.h>

#include <bgfx/bgfx.h>

namespace DFW
{
    namespace DRender
    {
        class RenderModule
        {
        public:
            RenderModule();
            ~RenderModule() = default;

            void InitRenderModule();
            void TerminateRenderModule();

            void ChangeGraphicsSettings(uint32 const a_bgfx_reset_flags);
            // ChangeRenderAPI - Experimental - Might cause issues.
            void ChangeRenderAPI(bgfx::RendererType::Enum a_render_type);

            void RenderFrame();

            void BeginFrame();
            void EndFrame();

            void SubmitMesh();
            void SubmitSprite();

            void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);

            void Debug_DrawBasicRenderInfo() const;
        private:
            bgfx::Init _bgfx_init_settings;

        };

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
