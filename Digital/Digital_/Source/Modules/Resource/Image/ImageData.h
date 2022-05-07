#pragma once

#include <string>

namespace DFW
{
    namespace DResource
    {
        struct ImageData
        {
            ImageData();
            ~ImageData();

            std::string file_name;
            std::string file_extension;
            int32 width;
            int32 height;
            int32 components_per_pixel;
            size_t data_size;
            uint8* data;

        };

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
