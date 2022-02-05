#pragma once

#include <Modules/ECS/Objects/ECSystem.h>

namespace DFW
{
    class RenderSystem : public DECS::System::StrictRegistrar<RenderSystem>
    {
    public:
        RenderSystem() = default;
        ~RenderSystem() = default;

        virtual void Init() override;
        virtual void Terminate() override;

        virtual void Update(DECS::Universe* const a_universe) override;

    };

} // End of namespace ~ DFW.
