#include <DFW/Modules/Resource/Image/ImageLoader.h>

#include <DFW/Modules/Resource/Image/ImageData.h>

#include <DFW/Utility/FileSystemUtility.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace DFW
{
    namespace DResource
    {
        DFW::UniquePtr<ImageData> LoadImageData(FilePath const& a_filepath)
        {
            DFW_ASSERT(a_filepath.IsValidPath());

            int32 image_width(0);
            int32 image_height(0);
            int32 image_components(0);
            stbi_uc* data = stbi_load(a_filepath.string().c_str(), &image_width, &image_height, &image_components, STBI_default);
            
            DFW::UniquePtr<ImageData> image;
            if (data)
            {
                image = DFW::MakeUnique<ImageData>();

                image->data         = data;
                image->data_size    = image_width * image_height * image_components * sizeof(stbi_uc);

                image->width        = static_cast<uint16>(image_width);
                image->height       = static_cast<uint16>(image_height);
                image->components_per_pixel = static_cast<uint8>(image_components);

                image->filepath     = a_filepath;
            }

            return image;
        }

        DFW::UniquePtr<ImageData> LoadImageDataFromMemory(uint8 const* a_image_data, size_t const a_image_size)
        {
            DFW_ASSERT(a_image_data != nullptr);
            DFW_ASSERT(a_image_size > 0);

            int32 image_width(0);
            int32 image_height(0);
            int32 image_components(0);
            stbi_uc* data = stbi_load_from_memory(static_cast<stbi_uc const*>(a_image_data), static_cast<int32>(a_image_size), &image_width, &image_height, &image_components, STBI_default);

            DFW::UniquePtr<ImageData> image;
            if (data)
            {
                image = DFW::MakeUnique<ImageData>();

                image->data         = data;
                image->data_size    = image_width * image_height * image_components * sizeof(stbi_uc);

                image->width        = static_cast<uint16>(image_width);
                image->height       = static_cast<uint16>(image_height);
                image->components_per_pixel = static_cast<uint8>(image_components);

                image->filepath     = FilePath("loaded-from-memory");
            }

            return image;
        }

        void UnloadImageData(ImageData* a_image_data)
        {
            stbi_image_free(a_image_data->data);
        }

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
