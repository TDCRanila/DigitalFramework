#pragma once

#include <DFW/GameWorld/Graphics/Renderable.h>

#include <DFW/Modules/ECS/Component.h>

#include <DFW/CoreSystems/Memory.h>

#include <DFW/Utility/ColourUtility.h>

namespace DFW
{
    // FW Declare.
    namespace DRender
    {
        struct MeshData;
        struct ShaderProgram;
    }

    SharedPtr<DRender::MeshData> Debug_CreateBasicCube();
    SharedPtr<DRender::MeshData> Debug_CreateBasicCube(DFW::ColourRGBA const& a_cube_colour);

    struct ModelComponent : public Renderable, DECS::Component::StrictRegistrar<ModelComponent>
    {
        ModelComponent() = default;
        ModelComponent(SharedPtr<DRender::MeshData> const& a_mesh, SharedPtr<DRender::ShaderProgram> const& a_shader) 
            : mesh(a_mesh), shader(a_shader) {}

        ~ModelComponent() = default;

        SharedPtr<DRender::MeshData> mesh;
        SharedPtr<DRender::ShaderProgram> shader;

    };

} // End of namespace ~ DFW.
