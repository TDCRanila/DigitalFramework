#include <GameWorld/Editor/GameViewport.h>

#include <GameWorld/Graphics/RenderSystem.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/ECSystemManager.h>

#include <Modules/Rendering/RenderModule.h>
#include <Modules/Rendering/RenderTargetDirector.h>

#include <CoreSystems/CoreServices.h>

namespace DFW
{
    namespace DEditor
    {
        void GameViewport::Init()
        {
            bgfx::TextureHandle texture_handle[2];
            texture_handle[0] = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
            texture_handle[1] = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::D0S8, BGFX_TEXTURE_RT | BGFX_TEXTURE_RT_WRITE_ONLY);
            bgfx::FrameBufferHandle const viewport_fbh = bgfx::createFrameBuffer(2, texture_handle, true);
            
            DRender::RenderTargetDirector& director = CoreService::GetRenderModule()->GetRenderTargetDirector();
            _viewport_render_target = director.RegisterRenderTarget(viewport_fbh, "viewport_render_target");

            CoreService::GetECS()->SystemManager().GetSystem<RenderSystem>()->RenderToRenderTarget(_viewport_render_target);
        }

        void GameViewport::Terminate()
        {
            CoreService::GetECS()->SystemManager().GetSystem<RenderSystem>()->RenderToDefault();

            DRender::RenderTargetDirector& director = CoreService::GetRenderModule()->GetRenderTargetDirector();
            director.FreeRenderTarget(*_viewport_render_target);
        }

        void GameViewport::Display()
        {
            ImGui::Begin(GetNameID().c_str(), &_is_viewport_window_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            
            // Draw the RenderTarget's texture on the whole window.
            _viewport_size = ImGui::GetContentRegionAvail();
            bgfx::TextureHandle const viewport_texture = bgfx::getTexture(_viewport_render_target->fbh);
            ImGui::Image(reinterpret_cast<void*>(viewport_texture.idx), _viewport_size);

            ImGui::End();
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
