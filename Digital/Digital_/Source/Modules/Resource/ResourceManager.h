#pragma once

#include <Modules/Resource/Resource.h>
#include <Modules/Resource/ResourceLoaders.h>

#include <entt/resource/cache.hpp>

namespace DFW
{
    namespace DResource
    {
        template <typename ResourceType, typename ResourceLoader>
        using ResourceCache = entt::resource_cache<ResourceType, ResourceLoader>;

        class ResourceManager
        {
        public:
            ResourceManager() = default;
            ~ResourceManager() = default;

            ResourceHandle<DResource::ImageData> LoadImageData(std::string const& a_filepath);
            ResourceHandle<DResource::ImageData> LoadImageData(std::string const& a_resource_identifer, uint8 const* a_data, size_t const a_data_size);

            ResourceHandle<DRender::TextureData> LoadTexture(std::string const& a_filepath);
            ResourceHandle<DRender::TextureData> LoadTexture(std::string const& a_resource_identifer, DResource::ImageData const* const a_image);
            
            ResourceHandle<DRender::MeshData> LoadMesh(std::string const& a_filepath);

            ResourceHandle<MapData> LoadMap(std::string const& a_filepath);

        private:
            struct ResourceID
            {
                ResourceID() : _id(DFW_INVALID_RESOUCRE_ID) {}
                ResourceID(std::string const& a_string_path) : _id(entt::hashed_string::value(a_string_path.c_str())) {}

                operator entt::id_type() const { return _id; }

                entt::id_type const _id;
            };

        private:
            ResourceCache<ImageData, ImageLoader> _image_cache;
            ResourceCache<DRender::TextureData, TextureLoader> _texture_cache;
            ResourceCache<DRender::MeshData, MeshLoader> _mesh_cache;
            ResourceCache<MapData, MapLoader> _map_cache;

        };

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
