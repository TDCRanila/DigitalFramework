#pragma once

#include <Modules/Rendering/ViewTargetDirector.h>
#include <Modules/Rendering/ViewTarget.h>

namespace DFW
{
    namespace DRender
    {
        // FW Declare.
        class RenderModuleContext;

        class RenderModule
        {
        public:
            RenderModule();
            ~RenderModule();

            void InitRenderModule();
            void TerminateRenderModule();

            void BeginFrame();
            void EndFrame();
            void RenderFrame();

            void SubmitMesh();
            void SubmitSprite();

            void ChangeRendererSettings();

            void Debug_RendererInfo(/*RenderModuleDebugOptions& const a_debug_option*/);

            ViewTargetDirector view_director;

        private:
            UniquePtr<RenderModuleContext> _render_module_context;

            SharedPtr<ViewTarget const> _main_view_target;

        };

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
