#include <Modules/Rendering/TextureLibrary.h>

namespace DFW
{
    namespace DRender
    {

        SharedPtr<TextureData> TextureLibrary::CreateTexture(DResource::ImageData const* const a_image_data)
        {
            bgfx::TextureFormat::Enum texture_format(bgfx::TextureFormat::Unknown);
            if (a_image_data->components_per_pixel == 3)
                texture_format = bgfx::TextureFormat::RGB8;
            else if (a_image_data->components_per_pixel == 4)
                texture_format = bgfx::TextureFormat::RGBA8;
            DFW_ASSERT(texture_format != bgfx::TextureFormat::Unknown);

            // Copied data will be freed by bgfx itself.
            bgfx::Memory const* data = bgfx::copy(a_image_data->data, static_cast<uint32>(a_image_data->data_size));

            bgfx::TextureHandle const texture_handle = bgfx::createTexture2D(a_image_data->width, a_image_data->height, false, 1, texture_format, 0, data);

            return MakeShared<TextureData>(texture_handle, a_image_data->width, a_image_data->height, BGFX_TEXTURE_NONE, uint8(0));
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.