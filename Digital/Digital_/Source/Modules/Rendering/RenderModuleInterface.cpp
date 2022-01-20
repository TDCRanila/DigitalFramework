#include <Modules/Rendering/RenderModuleInterface.h>

#include <Modules/Rendering/RenderModule_Impl_BGFX.h>

namespace DFW
{
    namespace DRender
    {
        SharedPtr<RenderModule> RenderModule::Construct()
        {
            // TODO Implement Platform IfDefs
            return MakeShared<RenderModuleBGFX>();
        }

        void RenderModule::InitRenderModule()
        {
        }

        void RenderModule::TerminateRenderModule()
        {
        }

        void RenderModule::RenderFrame()
        {
        }

        void RenderModule::BeginFrame()
        {
        }

        void RenderModule::EndFrame()
        {
        }

        void RenderModule::SubmitMesh()
        {
        }

        void RenderModule::SubmitSprite()
        {
        }

        void RenderModule::OnWindowResizeEvent(WindowResizeEvent const& a_window_event)
        {
        }

        void RenderModule::Debug_DrawBasicRenderInfo() const
        {
        }

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
