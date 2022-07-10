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
            uint16 width;
            uint16 height;
            uint8 components_per_pixel;
            size_t data_size;
            uint8* data;

        };

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
