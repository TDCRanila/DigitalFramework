#pragma once

#include <GameWorld/Graphics/BaseRenderSystem.h>

#include <Modules/ECS/System.h>

#include <CoreSystems/Memory.h>

namespace DFW
{
    struct SpriteBatch;

    class SpriteSystem final : public DECS::System::Registrar<SpriteSystem, BaseRenderSystem>
    {
    public:
        SpriteSystem() = default;
        virtual ~SpriteSystem() = default;

    private:
        virtual void Init() override;
        virtual void Terminate() override;

        virtual void Update(DECS::EntityRegistry& a_registry) override;

        void FlushSpriteBatch(SpriteBatch const& a_sprite_batch, DRender::ViewTarget const& a_view_target);
        
    };

} // End of namespace ~ DFW.
