#pragma once

#include <Modules/Rendering/TextureData.h>

#include <Modules/Resource/Image/ImageData.h>

#include <CoreSystems/Memory.h>

namespace DFW
{
    namespace DRender
    {
        class TextureLibrary
        {
        public:
            TextureLibrary() = default;
            ~TextureLibrary() = default;

        public:
            SharedPtr<TextureData> CreateTexture(DResource::ImageData const* const a_image_data);
            
        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
