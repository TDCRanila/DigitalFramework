#include <Modules/Rendering/RenderModule.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <Modules/Rendering/RenderModuleContext.h>

namespace DFW
{
    namespace DRender
    {
        RenderModule::RenderModule() = default;
        RenderModule::~RenderModule() = default;

        void RenderModule::InitRenderModule()
        {
            _render_module_context = MakeUnique<RenderModuleContext>();
            _render_module_context->InitRenderModuleContext();

            view_director.Init();
            _main_view_target = view_director.AllocateViewTarget(ViewTargetDirector::DEFAULT_MAIN_VIEWTARGET_NAME, DRender::ViewTargetInsertion::Front);

            CoreService::GetMainEventHandler()->InstantBroadcast<RendererInitializedEvent>();
        }

        void RenderModule::TerminateRenderModule()
        {
            shader_library.FreeLibraryResources();
            render_target_director.FreeAllRenderTargets();

            _render_module_context->TerminateRenderModuleContext();

            CoreService::GetMainEventHandler()->InstantBroadcast<RendererTerminatedEvent>();
        }
                          
        void RenderModule::BeginFrame()
        {
            _render_module_context->BeginFrame(*_main_view_target);
        }

        void RenderModule::EndFrame()
        {
            _render_module_context->EndFrame();
        }
                          
        void RenderModule::RenderFrame()
        {
            _render_module_context->RenderFrame();
        }

        void RenderModule::SubmitMesh()
        {
            _render_module_context->SubmitMesh();
        }
        
        void RenderModule::SubmitSprite()
        {
            _render_module_context->SubmitSprite();
        }
         
        void RenderModule::Debug_RendererInfo(/*RenderModuleDebugOptions& const a_debug_option*/)
        {
            // TODO Further Implement with differnt option.s
            _render_module_context->Debug_DrawBasicRenderInfo();
        }

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
