#include <DFW/GameWorld/Editor/GameViewport.h>

#include <DFW/Modules/ECS/ECSModule.h>

#include <DFW/Modules/Rendering/RenderModule.h>
#include <DFW/Modules/Rendering/RenderTargetDirector.h>
#include <DFW/Modules/Rendering/ViewTargetDirector.h>
#include <DFW/Modules/Rendering/RenderTarget.h>
#include <DFW/Modules/Rendering/ViewTarget.h>

#include <DFW/CoreSystems/CoreServices.h>

#include <bgfx/bgfx.h>

#include <imgui.h>

namespace DFW
{
    namespace DEditor
    {
        void GameViewport::Init()
        {
            // Create Viewport Framebuffer.
            bgfx::TextureHandle texture_handle[2];
            texture_handle[0] = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
            texture_handle[1] = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::D0S8, BGFX_TEXTURE_RT | BGFX_TEXTURE_RT_WRITE_ONLY);
            bgfx::FrameBufferHandle const viewport_fbh = bgfx::createFrameBuffer(2, texture_handle, true);
            
            // Create Viewport RenderTarget.
            SharedPtr<DRender::RenderModule> render_module =  CoreService::GetRenderModule();
            _viewport_render_target = render_module->GetRenderTargetDirector().RegisterRenderTarget(viewport_fbh, "viewport_render_target");

            // Create Viewport ViewTarget.
            _viewport_view_target = render_module->GetViewDirector().AllocateViewTarget("gameviewport", DRender::ViewTargetInsertion::Front);
            
        }

        void GameViewport::Terminate()
        {
            CoreService::GetRenderModule()->GetRenderTargetDirector().FreeRenderTarget(*_viewport_render_target);
        }

        void GameViewport::Display()
        {
            // Force Clear the Viewport ViewTarget.
            bgfx::ViewId const view_id = _viewport_view_target->view_target_id;
            bgfx::setViewRect(view_id, 0, 0, bgfx::BackbufferRatio::Equal);
            bgfx::setViewFrameBuffer(view_id, _viewport_render_target->fbh);
            bgfx::setViewClear(view_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
            bgfx::touch(view_id);

            // Draw the Viewport RenderTarget onto an ImGui Image.
            ImGui::Begin(GetNameID().c_str(), &_is_viewport_window_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            
            bgfx::TextureHandle const viewport_texture = bgfx::getTexture(_viewport_render_target->fbh);
            ImGui::Image(reinterpret_cast<void*>(viewport_texture.idx), ImGui::GetContentRegionAvail());

            ImGui::End();
        }

        SharedPtr<DRender::RenderTarget const> GameViewport::GetViewportRenderTarget() const
        {
            return _viewport_render_target;
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
