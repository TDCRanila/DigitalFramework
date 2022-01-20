#pragma once

#include <CoreSystems/ApplicationEvents.h>

namespace DFW
{
    //FW Declare.
    class ApplicationInstance;

    namespace DRender
    {
        class RenderModule
        {
            friend ApplicationInstance;
        public:
            RenderModule() = default;
            virtual ~RenderModule() = default;

            virtual void InitRenderModule();
            virtual void TerminateRenderModule();

            virtual void RenderFrame();

            virtual void BeginFrame();
            virtual void EndFrame();

            virtual void SubmitMesh();
            virtual void SubmitSprite();

            virtual void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);

            virtual void Debug_DrawBasicRenderInfo() const;
        private:
            static SharedPtr<RenderModule> Construct();

        };

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
