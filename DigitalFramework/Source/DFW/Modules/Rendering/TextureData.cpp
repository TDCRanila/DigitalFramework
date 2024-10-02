#include <DFW/Modules/Rendering/TextureData.h>

namespace DFW
{
    namespace DRender
    {
        TextureData::TextureData()
            : handle(BGFX_INVALID_HANDLE)
            , width(0)
            , height(0)
            , flags(0)
            , stage(0)
        {
        }

        TextureData::TextureData(bgfx::TextureHandle a_handle, int32 a_width, int32 a_height, uint64 a_flags, uint8 a_stage)
            : handle(a_handle)
            , width(a_width)
            , height(a_height)
            , flags(a_flags)
            , stage(a_stage)
        {
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
