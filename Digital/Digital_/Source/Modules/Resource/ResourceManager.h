#pragma once

#include <Modules/Resource/Resource.h>
#include <Modules/Resource/ResourceLoaders.h>

#include <Utility/TemplateUtility.h>

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

            template <typename ResourceType>
            ResourceHandle<ResourceType> Load(std::string const& a_filepath_or_identifer);

            ResourceHandle<DResource::ImageData> Load(std::string const& a_resource_identifer, uint8 const* a_image_data, size_t const a_image_data_size);
            ResourceHandle<DRender::TextureData> Load(std::string const& a_resource_identifer, DResource::ImageData const* const a_image);
            
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

#pragma region Template Function Implementation

        template <typename ResourceType>
        ResourceHandle<ResourceType> ResourceManager::Load(std::string const& a_filepath)
        {
            static_assert(IsAlwaysFalse<ResourceType>, __FUNCTION__ " - Trying to load of ResourceType which is not supported.");
            return ResourceHandle<ResourceType>();
        }

        template <>
        inline ResourceHandle<ImageData> ResourceManager::Load(std::string const& a_filepath)
        {
            auto const&& [it, has_emplaced_resource] = _image_cache.load(ResourceID(a_filepath), DResource::ImageLoader::from_disk_tag{}, a_filepath);
            return it->second;
        }

        template <>
        inline ResourceHandle<DRender::TextureData> ResourceManager::Load(std::string const& a_filepath)
        {
            ResourceID const id(a_filepath);
            auto const&& [image_it, has_emplaced_image] = _image_cache.load(id, DResource::ImageLoader::from_disk_tag{}, a_filepath);

            auto const&& [texture_it, has_emplaced_texture] = _texture_cache.load(id, image_it->second.handle().get());
            return texture_it->second;
        }

        template <>
        inline ResourceHandle<DRender::MeshData> ResourceManager::Load(std::string const& a_filepath)
        {
            auto const&& [it, has_emplaced_resource] = _mesh_cache.load(ResourceID(a_filepath), DResource::MeshLoader::from_disk_tag{}, a_filepath);
            return it->second;
        }

        template <>
        inline ResourceHandle<MapData> ResourceManager::Load(std::string const& a_filepath)
        {
            auto const&& [it, has_emplaced_resource] = _map_cache.load(ResourceID(a_filepath), MapLoader::from_disk_tag{}, a_filepath);
            return it->second;
        }

#pragma endregion

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
