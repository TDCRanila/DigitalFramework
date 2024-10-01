#pragma once

#include <CoreSystems/DUID.h>
#include <CoreSystems/Memory.h>

#include <bgfx/bgfx.h>

#include <vector>

namespace DFW
{
    namespace DRender
    {
        // FW Declare.
        struct RenderTarget;

        class RenderTargetDirector
        {
        public:
            RenderTargetDirector() = default;
            ~RenderTargetDirector() = default;


            SharedPtr<RenderTarget const> RegisterRenderTarget(bgfx::FrameBufferHandle const& a_framebuffer_handle, std::string const &a_render_target_name);
            void FreeRenderTarget(RenderTarget const& a_render_target);
            void FreeAllRenderTargets();

            SharedPtr<RenderTarget const> GetRenderTarget(std::string const& a_render_target_name) const;

            bool IsRenderTargetValid(RenderTarget const& a_render_target) const;

        private:
            std::vector<SharedPtr<RenderTarget const>>  _render_target_registration;

        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.

