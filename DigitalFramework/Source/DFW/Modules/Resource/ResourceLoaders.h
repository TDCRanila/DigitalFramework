#pragma once

#include <Modules/Resource/Image/ImageLoader.h>
#include <Modules/Resource/Mesh/MeshLoader.h>
#include <Modules/Resource/Map/MapBuilder.h>

#include <Modules/Rendering/RenderModule.h>
#include <Modules/Rendering/TextureLibrary.h>

#include <CoreSystems/CoreServices.h>

namespace DFW
{
    namespace DRender
    {
        struct TextureData;
        struct MeshData;
    } // End of namespace ~ DRender.

    namespace DResource
    {
        struct ImageData;
        struct MapData;
        
        class ImageLoader
        {
        public:
            using result_type = std::shared_ptr<ImageData>;

            struct from_disk_tag {};
            struct from_memory_tag {};

            template<typename ... Args>
            result_type operator()(from_disk_tag, Args&&... a_args)
            {
                return LoadImageData(std::forward<Args>(a_args)...);
            }

            template<typename ... Args>
            result_type operator()(from_memory_tag, Args&&... a_args) 
            {
                return LoadImageDataFromMemory(std::forward<Args>(a_args)...);
            }
        };

        class TextureLoader
        {
        public:
            using result_type = std::shared_ptr<DRender::TextureData>;

            template<typename ... Args>
            result_type operator()(Args&&... a_args)
            {
                return CoreService::GetRenderModule()->GetTextureLibrary().CreateTexture(std::forward<Args>(a_args)...);
            }
        };

        class MeshLoader
        {
        public:
            using result_type = std::shared_ptr<DRender::MeshData>;

            struct from_disk_tag {};

            template<typename ... Args>
            result_type operator()(from_disk_tag, Args&&... a_args)
            {
                return LoadMesh(std::forward<Args>(a_args)...);
            }
        };

        class MapLoader
        {
        public:
            using result_type = std::shared_ptr<MapData>;

            struct from_disk_tag {};

            template<typename ... Args>
            result_type operator()(from_disk_tag, Args&&... a_args)
            {
                MapBuilder builder;
                return builder.CreateMap(std::forward<Args>(a_args)...);
            }
        };

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
