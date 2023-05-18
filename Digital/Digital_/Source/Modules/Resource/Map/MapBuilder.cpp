#include <Modules/Resource/Map/MapBuilder.h>

#include <Modules/Resource/ResourceManager.h>

#include <CoreSystems/CoreServices.h>

#include <Utility/FileSystemUtility.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>

#include <libmap/entity.h>
#include <libmap/geo_generator.h>
#include <libmap/map_data.h>
#include <libmap/map_parser.h>
#include <libmap/surface_gatherer.h>

#include <glm/gtc/matrix_transform.hpp>

namespace DFW
{
    namespace DResource
    {
        namespace Detail
        {
            static bgfx::VertexLayout GetMeshVertexLayout()
            {
                static bgfx::VertexLayout mesh_vertex_layout;
                if (!mesh_vertex_layout.getStride())
                {
                    mesh_vertex_layout.begin();

                    mesh_vertex_layout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
                    mesh_vertex_layout.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);
                    mesh_vertex_layout.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);
                    mesh_vertex_layout.add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Float);

                    mesh_vertex_layout.end();
                }

                return mesh_vertex_layout;
            }

            static SharedPtr<DRender::TextureData> Debug_GetDefaultTexture()
            {
                static SharedPtr<DRender::TextureData> texture;
                if (!texture)
                {
                    bgfx::TextureHandle texture_handle;
                    int32 width(1), height(1);
                    std::array<uint8, 3> pixel_texture = { 255, 255, 255 };
                    bgfx::TextureFormat::Enum texture_format = bgfx::TextureFormat::RGB8;
                    bgfx::Memory const* data = bgfx::copy(pixel_texture.data(), static_cast<uint32>(pixel_texture.size() * sizeof(uint8)));
                    texture_handle = bgfx::createTexture2D(width, height, false, 1, texture_format, 0, data);

                    texture = MakeShared<DRender::TextureData>(texture_handle, width, height, BGFX_TEXTURE_NONE, uint8(0));
                }

                return texture;
            }

            constexpr float32 const LibMapInverseScale = 16.f;
            constexpr std::array<const char*, 5> SupportedTextureExtensionsList = { "png", "tga", "jpg", "jpeg", "bmp" };

            namespace EntityClassNames
            {
                constexpr std::string_view const entity_worldspawn = "worldspawn";
                constexpr std::string_view const entity_funcgroup = "func_group";

            } // End of namespace ~ EntityClassNames;

        } // End of namespace ~ Detail.

        UniquePtr<MapData> MapBuilder::CreateMap(std::string const& a_filepath)
        {
            _map_filepath = a_filepath;

            if (!LoadMap())
                return nullptr;

            BuildMap();

            return std::move(_map);
        }

        bool MapBuilder::LoadMap()
        {
            // Attempt to load the map data.
            if (!DUtility::DoesFileExist(_map_filepath))
            {   
                // Failed to find map data file.
                return false;
            }

            _loaded_libmap_data = MakeUnique<LMMapData>();

            LMMapParser map_parser(_loaded_libmap_data);
            if (!map_parser.load_from_path(_map_filepath.c_str()))
            {
                // Failed to load map data from file.
                return false;
            }

            LoadAndCacheTextures();
            SetupSurfaceUVCalculation();

            return true;
        }

        void MapBuilder::BuildMap()
        {
            LMGeoGenerator geometry_generator(_loaded_libmap_data);
            geometry_generator.run();

            _map = MakeUnique<MapData>();
            _map->file_path = _map_filepath;

            for (int32 index(0); index < _loaded_libmap_data->entity_count; ++index)
            {
                LMEntity& entity = _loaded_libmap_data->entities[index];
                ParseEntity(index, entity);
            }
        }

        void MapBuilder::ParseEntity(int32 const a_entity_id, LMEntity& a_entity)
        {
            const char* entity_classname = a_entity.get_property("classname");

            if (entity_classname == Detail::EntityClassNames::entity_worldspawn || entity_classname == Detail::EntityClassNames::entity_funcgroup)
            {
                BuildEntityVisualMesh(a_entity_id, a_entity);
                BuildEntityCollisionMesh(a_entity_id, a_entity);
            }

            // Other Entity Types.

        }

        void MapBuilder::BuildEntityVisualMesh(int32 const a_entity_id, LMEntity& a_entity)
        {
            // Loop per texture.
            // Gather the surfaces of each brush with that texture.
            // Create the mesh from ther brush/surface data.
            for (int32 texture_index(0); texture_index < _loaded_libmap_data->texture_count; texture_index++)
            {
                LMTextureData const& texture_data = _loaded_libmap_data->textures[texture_index];

                LMSurfaceGatherer surface_gatherer(_loaded_libmap_data);
                surface_gatherer.surface_gatherer_set_entity_index_filter(a_entity_id);
                surface_gatherer.surface_gatherer_set_texture_filter(texture_data.name);
                surface_gatherer.surface_gatherer_set_split_type(SST_BRUSH);
                surface_gatherer.surface_gatherer_run();

                LMSurfaces const& gathered_surfaces = surface_gatherer.out_surfaces;
                if (gathered_surfaces.surface_count == 0)
                    continue;

                for (int32 surface_index(0); surface_index < gathered_surfaces.surface_count; surface_index++)
                {
                    LMSurface const& surface = gathered_surfaces.surfaces[surface_index];
                    if (surface.vertex_count == 0)
                        continue;

                    constexpr uint32 indicies_per_face(3);
                    DRender::SubMeshData& submesh = _map->mesh_data.submeshes.emplace_back();
                    submesh.num_vertices = static_cast<uint32>(surface.vertex_count);
                    submesh.num_faces = static_cast<uint32>(surface.index_count / indicies_per_face);

                    submesh.vertex_layout = Detail::GetMeshVertexLayout();

                    // Buffers.
                    uint32 const layout_stride = submesh.vertex_layout.getStride();
                    uint32 const floats_per_stride(layout_stride / sizeof(float32));
                    submesh.vertices.reserve(submesh.num_vertices * floats_per_stride);
                    submesh.indices.reserve(surface.index_count);

                    // Vertices.
                    for (int32 vertex_index(0); vertex_index < surface.vertex_count; vertex_index++)
                    {
                        // Swap Z and Y coordinates as the Z is up in the .map format.
                        LMFaceVertex const& v = surface.vertices[vertex_index];
                        submesh.vertices.emplace_back(static_cast<float32>(v.vertex.x / Detail::LibMapInverseScale));
                        submesh.vertices.emplace_back(static_cast<float32>(v.vertex.z / Detail::LibMapInverseScale));
                        submesh.vertices.emplace_back(static_cast<float32>(v.vertex.y / Detail::LibMapInverseScale));
                        submesh.vertices.emplace_back(static_cast<float32>(v.uv.u));
                        submesh.vertices.emplace_back(static_cast<float32>(v.uv.v));
                        submesh.vertices.emplace_back(static_cast<float32>(v.normal.x));
                        submesh.vertices.emplace_back(static_cast<float32>(v.normal.z));
                        submesh.vertices.emplace_back(static_cast<float32>(v.normal.y));
                        submesh.vertices.emplace_back(static_cast<float32>(v.tangent.x));
                        submesh.vertices.emplace_back(static_cast<float32>(v.tangent.z));
                        submesh.vertices.emplace_back(static_cast<float32>(v.tangent.y));
                        submesh.vertices.emplace_back(static_cast<float32>(v.tangent.w));
                    }

                    // Indicies & Primitives.
                    for (int32 index(0); index < surface.index_count; index += 3)
                    {
                        // Change face cull winding from CounterClockWise -> ClockWise.
                        uint16 const index1 = submesh.indices.emplace_back(surface.indices[index]);
                        uint16 const index2 = submesh.indices.emplace_back(surface.indices[index + 2]);
                        uint16 const index3 = submesh.indices.emplace_back(surface.indices[index + 1]);

                        DRender::Primitive& primitive = submesh.primitives.emplace_back();
                        primitive.indices = { index1, index2, index3 };
                        primitive.vertex_indices = { index1 * floats_per_stride , index2 * floats_per_stride , index3 * floats_per_stride };

                    }

                    // Create Vertex and Index Buffers.
                    DFW_ASSERT(submesh.vertices.size() == surface.vertex_count * floats_per_stride);
                    uint32 vertex_data_size(static_cast<uint32>(submesh.vertices.size() * sizeof(decltype(submesh.vertices[0]))));
                    bgfx::Memory const* vertex_mem = bgfx::copy(submesh.vertices.data(), vertex_data_size);
                    submesh.vbh = bgfx::createVertexBuffer(vertex_mem, submesh.vertex_layout);

                    DFW_ASSERT(submesh.indices.size() == surface.index_count);
                    uint32 const index_data_size(static_cast<uint32>(submesh.indices.size() * sizeof(decltype(submesh.indices[0]))));
                    bgfx::Memory const* indices_mem = bgfx::copy(submesh.indices.data(), index_data_size);
                    submesh.ibh = bgfx::createIndexBuffer(indices_mem);

                    // Set the (cached) texture to mesh.
                    if (auto const& it = _cached_map_textures.find(texture_data.name); it != _cached_map_textures.end())
                    {
                        SharedPtr<DRender::TextureData> const& texture = it->second;
                        submesh.textures.emplace_back(texture);
                    }
                }
            }
        }

        void MapBuilder::BuildEntityCollisionMesh(int32 const a_entity_id, LMEntity& a_entity)
        {
            // Gather surface data of each brush.
            // Create collision mesh out of that data.
            LMSurfaceGatherer surface_gatherer(_loaded_libmap_data);
            surface_gatherer.surface_gatherer_set_entity_index_filter(a_entity_id);
            surface_gatherer.surface_gatherer_set_split_type(SST_BRUSH);
            surface_gatherer.surface_gatherer_run();

            LMSurfaces const& gathered_surfaces = surface_gatherer.out_surfaces;
            for (int32 surface_index(0); surface_index < gathered_surfaces.surface_count; surface_index++)
            {
                LMSurface const& surface = gathered_surfaces.surfaces[surface_index];
                if (surface.vertex_count == 0)
                    continue;

                JPH::VertexList vertex_list;
                vertex_list.reserve(surface.vertex_count);
                for (int32 vertex_index(0); vertex_index < surface.vertex_count; vertex_index++)
                {
                    vec3 const& vertex = surface.vertices[vertex_index].vertex;
                    // Swap Z and Y coordinates as the Z is up in the .map format.
                    vertex_list.emplace_back(
                          vertex.x / Detail::LibMapInverseScale
                        , vertex.z / Detail::LibMapInverseScale
                        , vertex.y / Detail::LibMapInverseScale
                    );
                }

                constexpr uint32 indicies_per_face(3);
                JPH::IndexedTriangleList index_list;
                index_list.reserve(surface.index_count / indicies_per_face);
                for (int32 index(0); index < surface.index_count; index += 3)
                {
                    uint16 const index1 = surface.indices[index];
                    uint16 const index2 = surface.indices[index + 1];
                    uint16 const index3 = surface.indices[index + 2];

                    index_list.emplace_back(JPH::IndexedTriangle(index1, index2, index3, 0));
                }

                // Create the collider shape from vertex and index lists.
                JPH::RefConst<JPH::ShapeSettings> mesh_shape = new JPH::MeshShapeSettings(vertex_list, index_list);

                // Get the position of the collider.
                glm::vec3 center(0.0f);
                if (a_entity.has_property("origin"))
                {
                    vec3 origin = a_entity.get_property_vec3("origin");
                    // Swap Z and Y coordinates as the Z is up in the .map format.
                    center = glm::vec3(origin.x, origin.z, origin.y);
                }

                MapData::ColliderData collider;
                collider.shape_settings = mesh_shape;
                collider.center_position = center;
                _map->collider_data.emplace_back(collider);
            }
        }

        void MapBuilder::LoadAndCacheTextures()
        {
            _cached_map_textures.reserve(_loaded_libmap_data->texture_count);

            SharedPtr<ResourceManager> const& resource_manager = CoreService::GetResourceManager();
            for (int32 index(0); index < _loaded_libmap_data->texture_count; ++index)
            {
                LMTextureData const& libmap_texture = _loaded_libmap_data->textures[index];
                std::string const texture_name(libmap_texture.name);

                bool const is_empty_texture = (texture_name == "") || (texture_name == "__TB_empty");

                std::filesystem::path image_path;
                if (!is_empty_texture)
                {
                    for (const char* file_extension : Detail::SupportedTextureExtensionsList)
                    {
                        // TODO: Change resource handling.
                        // For now, the map textures should be next to the .map file in the same folder.
                        std::string const check_path(DUtility::GetParentPath(_map_filepath) + DIR_SLASH + DUtility::GetFileName(texture_name) + '.' + file_extension);

                        if (DUtility::DoesFileExist(check_path))
                        {
                            image_path = check_path;
                            break;
                        }
                    }
                }

                bool const cannot_find_valid_texture = image_path.empty();
                if (is_empty_texture || cannot_find_valid_texture)
                {
                    _cached_map_textures.emplace(texture_name, Detail::Debug_GetDefaultTexture());
                    continue;
                }

                ResourceHandle<DRender::TextureData> const& texture = resource_manager->Load<DRender::TextureData>(image_path.string());
                _cached_map_textures.emplace(texture_name, texture.handle());
            }
        }

        void MapBuilder::SetupSurfaceUVCalculation()
        {
            if (_cached_map_textures.empty())
                return; // No textures have been loaded and cached or textures are present.

            // Setting the texture width and height for each loaded map texture, so that the geometry_generator can calculate the UVs.
            for (int32 index(0); index < _loaded_libmap_data->texture_count; index++) 
            {
                // Make sure we don't divide by 0 when calculating the UVs and produce NaN UV's.
                LMTextureData& map_texture_data = _loaded_libmap_data->textures[index];
                if (auto const& it = _cached_map_textures.find(map_texture_data.name); it != _cached_map_textures.end())
                {
                    SharedPtr<DRender::TextureData> const& loaded_texture = it->second;
                    map_texture_data.width = loaded_texture->width;
                    map_texture_data.height = loaded_texture->height;
                }
                else
                {
                    map_texture_data.width = 1;
                    map_texture_data.height = 1;
                }
            }
        }

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
