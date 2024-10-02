#pragma once

#include <GameWorld/Graphics/Renderable.h>

#include <Modules/Rendering/TextureData.h>
#include <Modules/ECS/Component.h>

#include <CoreSystems/Memory.h>

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
