#include <Modules/Rendering/TextureData.h>

namespace DFW
{
    namespace DRender
    {
        TextureData::TextureData()
            : handle(BGFX_INVALID_HANDLE)
            , flags(0)
            , stage(0)
        {
        }

        TextureData::TextureData(bgfx::TextureHandle a_handle, uint64 a_flags, uint8 a_stage)
            : handle(a_handle)
            , flags(a_flags)
            , stage(a_stage)
        {
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
