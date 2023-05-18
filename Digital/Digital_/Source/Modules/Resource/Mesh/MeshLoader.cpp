#include <Modules/Resource/Mesh/MeshLoader.h>

#include <Modules/Resource/ResourceManager.h>
#include <Modules/Rendering/TextureData.h>

#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/CoreServices.h>
#include <Utility/FileSystemUtility.h>

#include <Defines/MathDefines.h>

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <glm/vec3.hpp>

#include <array>

namespace DFW
{
    namespace DResource
    {
        namespace Detail
        {
            uint8 ColourFloat32ToUInt8(float32 const a_value)
            {
                return static_cast<uint8>(a_value * 255.f);
            }

            uint32 AIColorToUInt32(aiColor4D const& a_colour)
            {
                constexpr uint8 r_component_bit_shift = 0;
                constexpr uint8 g_component_bit_shift = 8;
                constexpr uint8 b_component_bit_shift = 16;
                constexpr uint8 a_component_bit_shift = 24;

                uint32 result(0);
                result  = static_cast<uint32>(ColourFloat32ToUInt8(a_colour.r)) << r_component_bit_shift;
                result |= static_cast<uint32>(ColourFloat32ToUInt8(a_colour.g)) << g_component_bit_shift;
                result |= static_cast<uint32>(ColourFloat32ToUInt8(a_colour.b)) << b_component_bit_shift;
                result |= static_cast<uint32>(ColourFloat32ToUInt8(a_colour.a)) << a_component_bit_shift;
                return result;
            }

            glm::vec3 AIVec3ToGlmVec3(aiVector3D a_assimp_vec3)
            {
                return glm::vec3(a_assimp_vec3.x, a_assimp_vec3.y, a_assimp_vec3.z);
            }

            struct AssimpMesh
            {
                AssimpMesh(aiMesh* a_mesh, uint32 a_index)
                    : mesh(a_mesh)
                    , index_in_scene(a_index)
                {
                }

                aiMesh* mesh;
                uint32 index_in_scene;
            };

            void ParseAssimpNode(aiNode const* a_assimp_node, aiScene const* a_assimp_scene, auto a_mesh_parser_func)
            {
                for (uint32 mesh_index(0); mesh_index < a_assimp_node->mNumMeshes; ++mesh_index)
                {
                    uint32 const mesh_index_in_scene(a_assimp_node->mMeshes[mesh_index]);
                    a_mesh_parser_func(AssimpMesh(a_assimp_scene->mMeshes[mesh_index_in_scene], mesh_index), a_assimp_scene);
                }

                for (uint32 child_node_index(0); child_node_index < a_assimp_node->mNumChildren; ++child_node_index)
                    ParseAssimpNode(a_assimp_node->mChildren[child_node_index], a_assimp_scene, a_mesh_parser_func);
            }

        } // End of namespace ~ Detail.

        UniquePtr<DRender::MeshData> LoadMesh(std::string const& a_filepath)
        {
            std::string const file_extension = DUtility::GetFileExtension(a_filepath);
            std::string const file_name = DUtility::GetFileStem(a_filepath);

            DFW_ASSERT(DUtility::DoesFileExist(a_filepath));

            Assimp::Importer assimp_importer;
            DFW_ASSERT(assimp_importer.IsExtensionSupported(file_extension));

            uint32 const assimp_process_flags(static_cast<uint32>(
                  aiPostProcessSteps::aiProcess_CalcTangentSpace
                | aiPostProcessSteps::aiProcess_Triangulate
                | aiPostProcessSteps::aiProcess_JoinIdenticalVertices
                | aiPostProcessSteps::aiProcess_SortByPType
                | aiPostProcessSteps::aiProcess_OptimizeGraph
                | aiPostProcessSteps::aiProcess_OptimizeMeshes
                | aiPostProcessSteps::aiProcess_MakeLeftHanded
                | aiPostProcessSteps::aiProcess_FlipUVs
                | aiPostProcessSteps::aiProcess_GenBoundingBoxes
                | aiPostProcessSteps::aiProcess_Triangulate
            ));

            // Import
            aiScene const* assimp_scene = assimp_importer.ReadFile(a_filepath, assimp_process_flags);
            DFW_ASSERT(assimp_scene);

            DFW_ASSERT(assimp_scene->HasMeshes() && "How?");

            // Allocate.
            UniquePtr<DRender::MeshData> mesh = MakeUnique<DRender::MeshData>();
            mesh->source_file = a_filepath;
            mesh->file_name = DUtility::GetFileName(a_filepath);

            // Parse all nodes starting from the root.
            auto AssimpMeshParser = [&mesh](Detail::AssimpMesh a_mesh, aiScene const* a_assimp_scene) -> void
            {
                // Setup.
                aiMesh const* assimp_mesh = a_mesh.mesh;

                // Add new submesh.
                DRender::SubMeshData& submesh = mesh->submeshes.emplace_back();

                if (!assimp_mesh->HasPositions() || !assimp_mesh->HasFaces())
                    DFW_ASSERT(false && "Must have vertex and index data.");

                bool const mesh_has_vertex_colours(assimp_mesh->HasVertexColors(0));
                if (mesh_has_vertex_colours)
                    DFW_ASSERT(assimp_mesh->GetNumColorChannels() <= 1 && "Not supported.");

                bool const mesh_has_texture_coords(assimp_mesh->HasTextureCoords(0));
                if (mesh_has_texture_coords)
                    DFW_ASSERT(assimp_mesh->GetNumUVChannels() <= 1 && "Not supported.");

                bool const mesh_has_normals(assimp_mesh->HasNormals());
                bool const mesh_has_tangents(assimp_mesh->HasTangentsAndBitangents());

                // Setup VertexLayout.
                {
                    submesh.vertex_layout.begin();

                    // VertexPosition Layout.
                    submesh.vertex_layout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);

                    // VertexColour Layout.
                    if (mesh_has_vertex_colours)
                        submesh.vertex_layout.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);

                    // TexCoord Layout.
                    if (mesh_has_texture_coords)
                        submesh.vertex_layout.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);

                    // Normal Layout.
                    if (mesh_has_normals)
                    {
                        submesh.vertex_layout.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);

                        // Tangent Layout.
                        if (mesh_has_tangents)
                        {
                            submesh.vertex_layout.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float);
                            submesh.vertex_layout.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float);
                        }
                    }

                    submesh.vertex_layout.end();
                }

                // Vertex & submesh.indices Count.
                submesh.num_vertices = static_cast<uint32>(assimp_mesh->mNumVertices);
                submesh.num_faces = static_cast<uint32>(assimp_mesh->mNumFaces);

                // Buffers.
                uint32 const layout_stride = submesh.vertex_layout.getStride();
                uint32 const floats_per_stride(layout_stride / sizeof(float32));
                submesh.vertices.reserve(submesh.num_vertices * floats_per_stride);

                uint32 const indices_per_face(assimp_mesh->mFaces[0].mNumIndices);
                submesh.indices.reserve(submesh.num_faces * indices_per_face);

                // submesh.vertices.
                for (uint32 mesh_vertex_index = 0; mesh_vertex_index < assimp_mesh->mNumVertices; ++mesh_vertex_index)
                {
                    {
                        aiVector3D const& vertex = assimp_mesh->mVertices[mesh_vertex_index];
                        submesh.vertices.emplace_back(vertex.x);
                        submesh.vertices.emplace_back(vertex.y);
                        submesh.vertices.emplace_back(vertex.z);
                    }

                    if (mesh_has_vertex_colours)
                    {
                        aiColor4D const& vertex_colour = assimp_mesh->mColors[0][mesh_vertex_index];
                        // uint32 explicitly represented as float32, same bytes.
                        submesh.vertices.emplace_back(static_cast<float32>(Detail::AIColorToUInt32(vertex_colour)));
                    }

                    if (mesh_has_texture_coords)
                    {
                        aiVector3D const& texture_coord = assimp_mesh->mTextureCoords[0][mesh_vertex_index];
                        submesh.vertices.emplace_back(texture_coord.x);
                        submesh.vertices.emplace_back(texture_coord.y);
                    }

                    if (mesh_has_normals)
                    {
                        aiVector3D const& normal = assimp_mesh->mNormals[mesh_vertex_index];
                        submesh.vertices.emplace_back(normal.x);
                        submesh.vertices.emplace_back(normal.y);
                        submesh.vertices.emplace_back(normal.z);
                    }

                    if (mesh_has_tangents)
                    {
                        aiVector3D const& tangent = assimp_mesh->mTangents[mesh_vertex_index];
                        submesh.vertices.emplace_back(tangent.x);
                        submesh.vertices.emplace_back(tangent.y);
                        submesh.vertices.emplace_back(tangent.z);

                        aiVector3D const& bittangent = assimp_mesh->mBitangents[mesh_vertex_index];
                        submesh.vertices.emplace_back(bittangent.x);
                        submesh.vertices.emplace_back(bittangent.y);
                        submesh.vertices.emplace_back(bittangent.z);
                    }
                }

                // submesh.indices
                for (uint32 mesh_face_index = 0; mesh_face_index < assimp_mesh->mNumFaces; ++mesh_face_index)
                {
                    aiFace const& face = assimp_mesh->mFaces[mesh_face_index];

                    DFW_ASSERT(face.mIndices[0] <= DMath::int16_max && "32-bit index buffer not support - See BGFX_BUFFER_INDEX32 flag.");
                    DFW_ASSERT(face.mIndices[1] <= DMath::int16_max && "32-bit index buffer not support - See BGFX_BUFFER_INDEX32 flag.");
                    DFW_ASSERT(face.mIndices[2] <= DMath::int16_max && "32-bit index buffer not support - See BGFX_BUFFER_INDEX32 flag.");
                    uint16 const index1(static_cast<uint16>(face.mIndices[0]));
                    uint16 const index2(static_cast<uint16>(face.mIndices[1]));
                    uint16 const index3(static_cast<uint16>(face.mIndices[2]));

                    submesh.indices.emplace_back(index1);
                    submesh.indices.emplace_back(index2);
                    submesh.indices.emplace_back(index3);

                    // Collect Primitives.
                    DRender::Primitive& primitive = submesh.primitives.emplace_back();
                    primitive.vertex_indices  = { 
                          index1 * floats_per_stride
                        , index2 * floats_per_stride
                        , index3 * floats_per_stride 
                    };

                    primitive.indices = { index1, index2, index3 };
                }

                // Create Vertex and Index Buffers.
                DFW_ASSERT(submesh.vertices.size() == submesh.num_vertices * floats_per_stride);
                uint32 vertex_data_size(static_cast<uint32>(submesh.vertices.size() * sizeof(decltype(submesh.vertices[0]))));
                bgfx::Memory const* vertex_mem = bgfx::copy(submesh.vertices.data(), vertex_data_size);
                submesh.vbh = bgfx::createVertexBuffer(vertex_mem, submesh.vertex_layout);

                DFW_ASSERT(submesh.indices.size() == submesh.num_faces * indices_per_face);
                uint32 const index_data_size(static_cast<uint32>(submesh.indices.size() * sizeof(decltype(submesh.indices[0]))));
                bgfx::Memory const* indices_mem = bgfx::copy(submesh.indices.data(), index_data_size);
                submesh.ibh = bgfx::createIndexBuffer(indices_mem);

                // AABB.
                glm::vec3 const aabb_min(Detail::AIVec3ToGlmVec3(assimp_mesh->mAABB.mMin));
                glm::vec3 const aabb_max(Detail::AIVec3ToGlmVec3(assimp_mesh->mAABB.mMax));
                submesh.aabb.min = aabb_min;
                submesh.aabb.max = aabb_max;

                // Spherical Collider.
                submesh.sphere.pos      = glm::vec3(0.0f);
                submesh.sphere.radius   = glm::length(glm::max(aabb_min, aabb_max));

                // TODO
                // OBB.

                // Materials
                if (a_assimp_scene->HasMaterials())
                {
                    aiMaterial const* assimp_material(a_assimp_scene->mMaterials[assimp_mesh->mMaterialIndex]);

                    // Mesh Colours
                    //aiColor3D material_colour;
                    //bool const is_simple_material(assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, material_colour) == aiReturn::aiReturn_SUCCESS);
                    //if (is_simple_material)
                    //{
                    //    // For now create a 1x1 texture.
                    //
                    //    std::array<uint8, 3> pixel_texture = 
                    //    { 
                    //          Detail::ColourFloat32ToUInt8(material_colour.r)
                    //        , Detail::ColourFloat32ToUInt8(material_colour.g)
                    //        , Detail::ColourFloat32ToUInt8(material_colour.b) 
                    //    };
                    //
                    //    bgfx::TextureFormat::Enum texture_format = bgfx::TextureFormat::RGB8;
                    //    bgfx::Memory const* data = bgfx::copy(pixel_texture.data(), pixel_texture.size() * sizeof(uint8));
                    //    bgfx::TextureHandle texture_handle = bgfx::createTexture2D(1, 1, false, 1, texture_format, 0, data);
                    //
                    //    static bgfx::UniformHandle sampler_texture(BGFX_INVALID_HANDLE);
                    //    if (!bgfx::isValid(sampler_texture))
                    //        sampler_texture = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
                    //
                    //    submesh.textures.emplace_back(MakeShared<DRender::TextureData>(texture_handle, sampler_texture, BGFX_TEXTURE_NONE, 0));
                    //}
                    //else
                    //{
                    auto ParseMaterial = [&a_assimp_scene, &mesh](aiMaterial const* a_assimp_material, aiTextureType const a_texture_type) -> std::vector<SharedPtr<DRender::TextureData>>
                    {
                        std::vector<SharedPtr<DRender::TextureData>> material_textures;
                        
                        uint32 const texture_count(a_assimp_material->GetTextureCount(a_texture_type));
                        for (uint32 texture_index(0); texture_index < texture_count; ++texture_index)
                        {
                            // Check Texture.
                            aiString texture_path;
                            aiReturn result = a_assimp_material->GetTexture(a_texture_type, texture_index, &texture_path);
                            DFW_ASSERT(result == aiReturn::aiReturn_SUCCESS);

                            // Check if embedded.
                            auto [assimp_texture_data, index] = a_assimp_scene->GetEmbeddedTextureAndIndex(texture_path.C_Str());
                            bool const is_texture_embedded(assimp_texture_data);

                            // Load Image.
                            SharedPtr<ResourceManager> const& resource_manager = CoreService::GetResourceManager();
                            ResourceHandle<DRender::TextureData> texture_data;
                            if (is_texture_embedded)
                            {
                                auto get_extension = [](char const chars[HINTMAXTEXTURELEN]) -> std::string
                                {
                                    std::string extension("");
                                    for (int32 index(0); index < HINTMAXTEXTURELEN; ++index)
                                    {
                                        char const c = chars[index];
                                        switch (c)
                                        {
                                        case('\\0'):
                                            break;
                                        default:
                                            extension += c;
                                            break;
                                        }
                                    }

                                    return extension;
                                };

                                bool const is_texture_compressed(assimp_texture_data->mHeight == 0);
                                
                                size_t texture_size(0);
                                if (is_texture_compressed)
                                    texture_size = static_cast<size_t>(assimp_texture_data->mWidth);
                                else
                                    texture_size = static_cast<size_t>(assimp_texture_data->mWidth * assimp_texture_data->mHeight * sizeof(aiTexel));

                                std::string const embedded_file_extension = "." + get_extension(assimp_texture_data->achFormatHint);
                                std::string const embedded_file_name = DUtility::GetFileStem(assimp_texture_data->mFilename.C_Str());

                                ResourceHandle<ImageData> const image_data = resource_manager->Load(embedded_file_name, reinterpret_cast<uint8 const*>(assimp_texture_data->pcData), texture_size);
                                texture_data = resource_manager->Load(embedded_file_name, image_data.handle().get());
                            }
                            else
                            {
                                std::string const image_path(DUtility::GetParentPath(mesh->source_file) + DIR_SLASH + texture_path.C_Str());
                                texture_data = resource_manager->Load<DRender::TextureData>(image_path);
                            }

                            // Store Texture.
                            DFW_ASSERT(texture_data);
                            material_textures.emplace_back(texture_data.handle());
                        }

                        return material_textures;
                    };

                    // Diffuse.
                    std::vector<SharedPtr<DRender::TextureData>> const& diffuse_textures = ParseMaterial(assimp_material, aiTextureType::aiTextureType_DIFFUSE);
                    if (!diffuse_textures.empty())
                        submesh.textures.insert(submesh.textures.end(), diffuse_textures.begin(), diffuse_textures.end());
                }
            };

            Detail::ParseAssimpNode(assimp_scene->mRootNode, assimp_scene, AssimpMeshParser);
            return mesh;
        }

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
