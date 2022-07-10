#pragma once

#include <GameWorld/Graphics/BaseRenderSystem.h>

#include <Modules/ECS/Objects/ECSystem.h>

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
        virtual void Init() override;
        virtual void Terminate() override;

        virtual void Update(DECS::Universe& a_universe) override;

    };

} // End of namespace ~ DFW.
