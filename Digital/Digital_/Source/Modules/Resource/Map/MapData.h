#pragma once

#include <Modules/Rendering/MeshData.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

#include <glm/vec3.hpp>

#include <string>

namespace DFW
{
    namespace DResource
    {
        struct MapData
        {
            struct ColliderData
            {
                JPH::RefConst<JPH::ShapeSettings> shape_settings;
                glm::vec3 center_position;
            };

            DFW::DRender::MeshData mesh_data;
            std::vector<ColliderData> collider_data;

            std::string file_path;

        };

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
