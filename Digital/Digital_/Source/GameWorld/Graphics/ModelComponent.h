#pragma once

#include <GameWorld/Graphics/Renderable.h>

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

    struct ModelComponent : public Renderable, DECS::Component::StrictRegistrar<ModelComponent>
    {
        ModelComponent() = default;
        ~ModelComponent() = default;

        SharedPtr<DRender::MeshData> mesh;
        SharedPtr<DRender::ShaderProgram> shader;

    };

} // End of namespace ~ DFW.
