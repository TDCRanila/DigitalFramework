#pragma once

#include <Modules/Rendering/MeshData.h>

#include <Modules/Resource/Resource.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

#include <glm/vec3.hpp>

namespace DFW
{
    namespace DResource
    {
        struct MapData : Resource
        {
            struct ColliderData
            {
                ColliderData() : center_position(0.0f) {}

                JPH::RefConst<JPH::ShapeSettings> shape_settings;
                glm::vec3 center_position;
            };

            DFW::DRender::MeshData mesh_data;
            std::vector<ColliderData> collider_data;

        };

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
