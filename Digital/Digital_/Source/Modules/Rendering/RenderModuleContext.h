#pragma once

#include <CoreSystems/ApplicationEvents.h>

#include <bgfx/bgfx.h>

namespace DFW
{
    namespace DRender
    {
        struct ViewTarget;

        class RenderModuleContext
        {
        public:
            RenderModuleContext();
            ~RenderModuleContext() = default;

            void InitRenderModuleContext();
            void TerminateRenderModuleContext();

            void BeginFrame(SharedPtr<ViewTarget const> const& a_main_viewtarget);
            void EndFrame();
            void RenderFrame();

            void SubmitMesh();
            void SubmitSprite();

            bgfx::RendererType::Enum GetRenderAPIType() const;
            // ChangeRenderAPI - Experimental - Might cause issues.
            void ChangeRenderAPI(bgfx::RendererType::Enum a_render_type);
            void ChangeGraphicsSettings(uint32 const a_bgfx_reset_flags);

            void Debug_DrawBasicRenderInfo() const;

        private:
            void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);

            bgfx::Init _bgfx_init_settings;

        };

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
