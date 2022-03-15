#pragma once

#include <Modules/Math/Geometry.h>

#include <bgfx/bgfx.h>

#include <vector>

namespace DFW
{
    namespace DRender
    {
        struct Primitive
        {
            Primitive();

            uint32 start_vertices;
            uint32 start_index;
            uint32 num_vertices;
            uint32 num_indices;
            
            DMath::OBB obb;
            DMath::AABB aabb;
            DMath::Sphere sphere;

        };

        struct SubModelData
        {
            SubModelData();

            uint16* vertices;
            uint16* indices;
            uint32  num_vertices;
            uint32  num_indices;

            bgfx::VertexBufferHandle vbh;
            bgfx::IndexBufferHandle ibh;

            DMath::OBB obb;
            DMath::AABB aabb;
            DMath::Sphere sphere;

            std::vector<Primitive> primitives;

        };

        struct ModelData
        {
            bgfx::VertexLayout vertex_layout;
            std::vector<SubModelData> submodels;

        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
