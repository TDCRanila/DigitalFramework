#include <Modules/Resource/Image/ImageLoader.h>

#include <Modules/Resource/Image/ImageData.h>

#include <Utility/FileSystemUtility.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace DFW
{
    namespace DResource
    {
        DFW::UniquePtr<ImageData> LoadImageData(std::string const& a_filepath)
        {
            DFW_ASSERT(DUtility::DoesFileExist(a_filepath));

            int32 image_width(0);
            int32 image_height(0);
            int32 image_components(0);
            stbi_uc* data = stbi_load(a_filepath.c_str(), &image_width, &image_height, &image_components, STBI_default);
            
            DFW::UniquePtr<ImageData> image;
            if (data)
            {
                image = DFW::MakeUnique<ImageData>();

                image->data         = data;
                image->data_size    = image_width * image_height * image_components * sizeof(stbi_uc);

                image->width        = image_width;
                image->height       = image_height;
                image->components_per_pixel = image_components;

                image->file_name        = DUtility::GetFileStem(a_filepath);
                image->file_extension   = DUtility::GetFileExtension(a_filepath);
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
            stbi_uc* data = stbi_load_from_memory(static_cast<stbi_uc const*>(a_image_data), a_image_size, &image_width, &image_height, &image_components, STBI_default);

            DFW::UniquePtr<ImageData> image;
            if (data)
            {
                image = DFW::MakeUnique<ImageData>();

                image->data         = data;
                image->data_size    = image_width * image_height * image_components * sizeof(stbi_uc);

                image->width        = image_width;
                image->height       = image_height;
                image->components_per_pixel = image_components;

                image->file_name        = "loaded-from-memory";
                image->file_extension   = "loaded-from-memory";
            }

            return image;
        }

        void UnloadImageData(ImageData* a_image_data)
        {
            stbi_image_free(a_image_data->data);
        }

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
