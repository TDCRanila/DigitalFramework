#include <Modules/Resource/ResourceManager.h>

#include <entt/resource/fwd.hpp>
#include <entt/resource/resource.hpp>
#include <entt/resource/loader.hpp>
#include <entt/resource/cache.hpp>

namespace DFW
{
    namespace DResource
    {
        ResourceHandle<DResource::ImageData> ResourceManager::LoadImageData(std::string const& a_filepath)
        {
            auto const&& [it, has_emplaced_resource] = _image_cache.load(ResourceID(a_filepath), DResource::ImageLoader::from_disk_tag{}, a_filepath);
            return it->second;
        }

        ResourceHandle<DResource::ImageData> ResourceManager::LoadImageData(std::string const& a_resource_identifer, uint8 const* a_data, size_t const a_data_size)
        {
            ResourceID const id(a_resource_identifer);
            auto const&& [it, has_emplaced_resource] = _image_cache.load(id, DResource::ImageLoader::from_memory_tag{}, a_data, a_data_size);
            return it->second;
        }

        ResourceHandle<DRender::TextureData> ResourceManager::LoadTexture(std::string const& a_filepath)
        {
            ResourceID const id(a_filepath);
            auto const&& [image_it, has_emplaced_image] = _image_cache.load(id, DResource::ImageLoader::from_disk_tag{}, a_filepath);

            auto const&& [texture_it, has_emplaced_texture] = _texture_cache.load(id, image_it->second.handle().get());
            return texture_it->second;
        }

        ResourceHandle<DRender::TextureData> ResourceManager::LoadTexture(std::string const& a_resource_identifer, DResource::ImageData const* const a_image)
        {
            ResourceID const id(a_resource_identifer);
            auto const&& [it, has_emplaced_resource] = _texture_cache.load(id, a_image);
            return it->second;
        }

        ResourceHandle<DRender::MeshData> ResourceManager::LoadMesh(std::string const& a_filepath)
        {
            ResourceID const id(a_filepath);
            auto const&& [it, has_emplaced_resource] = _mesh_cache.load(id, DResource::MeshLoader::from_disk_tag{}, a_filepath);
            return it->second;
        }

        ResourceHandle<MapData> ResourceManager::LoadMap(std::string const& a_filepath)
        {
            ResourceID const id(a_filepath);
            auto const&& [it, has_emplaced_resource] = _map_cache.load(id, MapLoader::from_disk_tag{}, a_filepath);
            return it->second;
        }

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
