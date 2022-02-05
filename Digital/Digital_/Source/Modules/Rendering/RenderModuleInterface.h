#pragma once

#include <CoreSystems/ApplicationEvents.h>

namespace DFW
{
    namespace DRender
    {
        class RenderModule
        {
        public:
            RenderModule() = default;
            ~RenderModule() = default;

            void InitRenderModule();
            void TerminateRenderModule();

            void RenderFrame();

            void BeginFrame();
            void EndFrame();

            void SubmitMesh();
            void SubmitSprite();

            void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);

            void Debug_DrawBasicRenderInfo() const;

        };

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
