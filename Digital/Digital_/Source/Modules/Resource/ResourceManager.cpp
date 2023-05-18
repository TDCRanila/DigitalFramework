#include <Modules/Resource/ResourceManager.h>

#include <entt/resource/fwd.hpp>
#include <entt/resource/resource.hpp>
#include <entt/resource/loader.hpp>
#include <entt/resource/cache.hpp>

namespace DFW
{
    namespace DResource
    {
        ResourceHandle<DResource::ImageData> ResourceManager::Load(std::string const& a_resource_identifer, uint8 const* a_data, size_t const a_data_size)
        {
            auto const&& [it, has_emplaced_resource] = _image_cache.load(ResourceID(a_resource_identifer), DResource::ImageLoader::from_memory_tag{}, a_data, a_data_size);
            return it->second;
        }

        ResourceHandle<DRender::TextureData> ResourceManager::Load(std::string const& a_resource_identifer, DResource::ImageData const* const a_image)
        {
            auto const&& [it, has_emplaced_resource] = _texture_cache.load(ResourceID(a_resource_identifer), a_image);
            return it->second;
        }

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
