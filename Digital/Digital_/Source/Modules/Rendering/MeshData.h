#pragma once

#include <Modules/Rendering/TextureData.h>
#include <Modules/Math/Geometry.h>

#include <bgfx/bgfx.h>

#include <vector>
#include <array>

namespace DFW
{
    namespace DRender
    {
        struct Primitive
        {
            Primitive() = default;
            ~Primitive() = default;

            std::array<uint32, 3> vertex_indices;
            std::array<uint16, 3> indices;            
        };

        struct SubMeshData
        {
            SubMeshData();
            ~SubMeshData() = default;

            std::vector<float32> vertices;
            std::vector<uint16> indices;
            uint32  num_vertices;
            uint32  num_faces;

            bgfx::VertexLayout vertex_layout; // TODO: Check if the vertexlayout can be in MeshData and be shared between submeshes?
            bgfx::VertexBufferHandle vbh;
            bgfx::IndexBufferHandle ibh;

            DMath::OBB obb;
            DMath::AABB aabb;
            DMath::Sphere sphere;

            std::vector<SharedPtr<DRender::TextureData>> textures;
            std::vector<Primitive> primitives;
        };

        struct MeshData
        {
            MeshData();
            ~MeshData() = default;

            std::vector<SubMeshData> submeshes;
            std::string file_name;
            std::string source_file;

        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
