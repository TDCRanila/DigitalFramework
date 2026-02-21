#pragma once

#include <DFW/Modules/Resource/Resource.h>

#include <bgfx/bgfx.h>

namespace DFW
{
    namespace DRender
    {
        struct TextureData : public Resource
        {
            TextureData();
            TextureData(bgfx::TextureHandle a_handle, int32 a_width, int32 a_height, uint64 a_flags, uint8 a_stage);

            bgfx::TextureHandle handle;
            int32 width;
            int32 height;
            uint64 flags;
            uint8 stage;
        };
        
    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
