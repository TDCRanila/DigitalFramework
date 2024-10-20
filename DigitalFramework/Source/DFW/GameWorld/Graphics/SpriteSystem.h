#pragma once

#include <DFW/GameWorld/Graphics/BaseRenderSystem.h>

#include <DFW/Modules/ECS/System.h>

#include <DFW/CoreSystems/Memory.h>

namespace DFW
{
    struct SpriteBatch;

    class SpriteSystem final : public DECS::System::Registrar<SpriteSystem, BaseRenderSystem>
    {
    public:
        SpriteSystem() = default;
        virtual ~SpriteSystem() = default;

    private:
        virtual void Init(DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DECS::EntityRegistry& a_registry) override;

        virtual void Update(DECS::EntityRegistry& a_registry) override;

        void FlushSpriteBatch(SpriteBatch const& a_sprite_batch);
        
    };

} // End of namespace ~ DFW.
