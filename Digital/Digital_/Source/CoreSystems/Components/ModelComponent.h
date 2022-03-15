#pragma once

#include <Modules/ECS/Objects/ECSComponent.h>

#include <Modules/Rendering/TextureData.h>

#include <CoreSystems/Memory.h>

namespace DFW
{
    // FW Declare.
    namespace DRender
    {
        struct ModelData;
        struct ShaderProgram;
    }

    SharedPtr<DRender::ModelData> CreateBasicCube();

    struct ModelComponent : public DECS::Component::StrictRegistrar<ModelComponent>
    {
        ModelComponent();
        ~ModelComponent() = default;

        SharedPtr<DRender::ModelData> model;
        SharedPtr<DRender::ShaderProgram> shader;

        bool is_visible;

    };

} // End of namespace ~ DFW.
