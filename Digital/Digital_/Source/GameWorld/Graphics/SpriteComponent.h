#pragma once

#include <Modules/Rendering/TextureData.h>
#include <Modules/ECS/Objects/ECSComponent.h>

#include <CoreSystems/Memory.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace DFW
{
    struct SpriteComponent final : public DECS::Component::StrictRegistrar<SpriteComponent>
    {
    public:
        SpriteComponent();
        virtual ~SpriteComponent() = default;

        SharedPtr<DRender::TextureData> texture;
        glm::vec4 colour;
        bool is_visible = true;
    };

} // End of namespace ~ DFW.
