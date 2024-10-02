#pragma once

#include <Modules/Resource/Resource.h>

namespace DFW
{
    namespace DResource
    {
        struct ImageData : public Resource
        {
            ImageData();
            ~ImageData();

            uint16 width;
            uint16 height;
            uint8 components_per_pixel;
            size_t data_size;
            uint8* data;

        };

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
