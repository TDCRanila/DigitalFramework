#pragma once

#include <GameWorld/Graphics/BaseRenderSystem.h>

#include <Modules/ECS/System.h>

#include <CoreSystems/Memory.h>

namespace DFW
{
    struct CameraComponent;

    class RenderSystem final : public DECS::System::Registrar<RenderSystem, BaseRenderSystem>
    {
    public:
        RenderSystem() = default;
        virtual ~RenderSystem() = default;

    private:
        virtual void Init(DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DECS::EntityRegistry& a_registry) override;

        virtual void Update(DECS::EntityRegistry& a_registry) override;

    };

} // End of namespace ~ DFW.
