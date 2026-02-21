#pragma once

#include <DFW/CoreSystems/Memory.h>

#include <DFW/Utility/FilePath.h>

namespace DFW
{
    namespace DResource
    {
        // FW Declare.
        struct ImageData;

        DFW::UniquePtr<ImageData> LoadImageData(FilePath const& a_filepath);
        DFW::UniquePtr<ImageData> LoadImageDataFromMemory(uint8 const* a_image_data, size_t const a_image_size);

        void UnloadImageData(ImageData* a_image_data);

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
