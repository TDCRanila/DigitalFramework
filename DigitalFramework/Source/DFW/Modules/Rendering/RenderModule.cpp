#include <DFW/Modules/Rendering/RenderModule.h>

#include <DFW/CoreSystems/CoreServices.h>
#include <DFW/CoreSystems/Events/EventDispatcher.h>

#include <DFW/Modules/Rendering/ShaderLibrary.h>
#include <DFW/Modules/Rendering/UniformLibrary.h>
#include <DFW/Modules/Rendering/TextureLibrary.h>
#include <DFW/Modules/Rendering/ViewTargetDirector.h>
#include <DFW/Modules/Rendering/RenderModuleContext.h>
#include <DFW/Modules/Rendering/RenderTargetDirector.h>

namespace DFW
{
    namespace DRender
    {
        RenderModule::RenderModule() = default;
        RenderModule::~RenderModule() = default;

        void RenderModule::Init()
        {
            _render_module_context = MakeUnique<RenderModuleContext>();
            _render_module_context->InitRenderModuleContext();

            _view_director          = MakeUnique<ViewTargetDirector>();
            _render_target_director = MakeUnique<RenderTargetDirector>();
            _shader_library         = MakeUnique<ShaderLibrary>();
            _uniform_library        = MakeUnique<UniformLibrary>();
            _texture_library        = MakeUnique<TextureLibrary>();

            // Create the main view target.
            _view_director->Init();
            _main_view_target = _view_director->AllocateViewTarget(
                    ViewTargetDirector::DEFAULT_MAIN_VIEWTARGET_NAME, DRender::ViewTargetInsertion::Front);

            CoreService::GetAppEventHandler()->InstantBroadcast(RendererInitializedEvent());
        }

        void RenderModule::Terminate()
        {
            _shader_library->FreeLibraryResources();
            _render_target_director->FreeAllRenderTargets();

            _render_module_context->TerminateRenderModuleContext();

            _view_director.reset();
            _render_target_director.reset();
            _shader_library.reset();
            _uniform_library.reset();
            _texture_library.reset();
            
            _render_module_context.reset();

            CoreService::GetAppEventHandler()->InstantBroadcast(RendererTerminatedEvent());
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

        ViewTargetDirector& RenderModule::GetViewDirector() const
        {
            return *_view_director.get();
        }

        RenderTargetDirector& RenderModule::GetRenderTargetDirector() const
        {
            return *_render_target_director.get();
        }

        ShaderLibrary& RenderModule::GetShaderLibrary() const
        {
            return *_shader_library.get();
        }

        UniformLibrary& RenderModule::GetUniformLibrary() const
        {
            return *_uniform_library.get();
        }

        TextureLibrary& RenderModule::GetTextureLibrary() const
        {
            return *_texture_library.get();
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
            // TODO Further Implement with differnt options.
            _render_module_context->Debug_DrawBasicRenderInfo();
        }

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
