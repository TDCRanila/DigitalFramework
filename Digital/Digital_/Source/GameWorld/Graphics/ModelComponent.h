#pragma once

#include <Modules/ECS/Objects/ECSComponent.h>

#include <CoreSystems/Memory.h>

namespace DFW
{
    // FW Declare.
    namespace DRender
    {
        struct MeshData;
        struct ShaderProgram;
    }

    SharedPtr<DRender::MeshData> Debug_CreateBasicCube();

    struct ModelComponent : public DECS::Component::StrictRegistrar<ModelComponent>
    {
        ModelComponent();
        ~ModelComponent() = default;

        SharedPtr<DRender::MeshData> model;
        SharedPtr<DRender::ShaderProgram> shader;

        bool is_visible;

    };

} // End of namespace ~ DFW.
