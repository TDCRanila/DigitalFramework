#include <Modules/Resource/Image/ImageData.h>

#include <Modules/Resource/Image/ImageLoader.h>

namespace DFW
{
    namespace DResource
    {
        ImageData::ImageData()
            : width(0)
            , height(0)
            , components_per_pixel(0)
            , data_size(0)
            , data(nullptr)
        {
        }

        ImageData::~ImageData()
        {
            UnloadImageData(this);
        }

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
