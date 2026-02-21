#pragma once

#include <bgfx/bgfx.h>

#include <string>

namespace DFW
{
    namespace DRender
    {
        struct RenderTarget
        {
            RenderTarget(bgfx::FrameBufferHandle const a_fbh, std::string const& a_render_target_name);
            ~RenderTarget() = default;

            bgfx::FrameBufferHandle fbh;
            std::string name;
        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
