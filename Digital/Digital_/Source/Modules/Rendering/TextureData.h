#pragma once

#include <bgfx/bgfx.h>

namespace DFW
{
    namespace DRender
    {
        struct TextureData
        {
            TextureData();
            TextureData(bgfx::TextureHandle a_handle, uint64 a_flags, uint8 a_stage);

            bgfx::TextureHandle handle;
            uint64 flags;
            uint8 stage;
        };
        
    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
