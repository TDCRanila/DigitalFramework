#pragma once

#include <DFW/Modules/Rendering/TextureData.h>

#include <DFW/Modules/Resource/Image/ImageData.h>

#include <DFW/CoreSystems/Memory.h>

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
