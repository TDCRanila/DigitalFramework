#pragma once

#include <CoreSystems/ApplicationEvents.h>

#include <Modules/Rendering/ViewTargetDirector.h>

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

            void BeginFrame();
            void EndFrame();
            void RenderFrame();

            void SubmitMesh();
            void SubmitSprite();

            // ChangeRenderAPI - Experimental - Might cause issues.
            void ChangeRenderAPI(bgfx::RendererType::Enum a_render_type);
            void ChangeGraphicsSettings(uint32 const a_bgfx_reset_flags);

            void Debug_DrawBasicRenderInfo() const;

            ViewTargetDirector view_director;

        private:
            void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);

            bgfx::Init _bgfx_init_settings;

        };

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
