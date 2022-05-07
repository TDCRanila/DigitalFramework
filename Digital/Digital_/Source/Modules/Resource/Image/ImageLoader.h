#pragma once

#include <CoreSystems/Memory.h>

#include <string>

namespace DFW
{
    namespace DResource
    {
        // FW Declare.
        struct ImageData;

        DFW::UniquePtr<ImageData> LoadImageData(std::string const& a_filepath);
        DFW::UniquePtr<ImageData> LoadImageDataFromMemory(uint8 const* a_image_data, size_t const a_image_size);

        void UnloadImageData(ImageData* a_image_data);

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
