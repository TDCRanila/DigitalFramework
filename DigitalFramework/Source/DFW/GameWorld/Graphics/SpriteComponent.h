#pragma once

#include <DFW/GameWorld/Graphics/Renderable.h>

#include <DFW/Modules/Rendering/TextureData.h>
#include <DFW/Modules/ECS/Component.h>

#include <DFW/CoreSystems/Memory.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace DFW
{
    struct SpriteComponent final : public Renderable, DECS::Component::StrictRegistrar<SpriteComponent>
    {
    public:
        SpriteComponent() = default;
        virtual ~SpriteComponent() = default;

        SharedPtr<DRender::TextureData> texture;
    };

} // End of namespace ~ DFW.
