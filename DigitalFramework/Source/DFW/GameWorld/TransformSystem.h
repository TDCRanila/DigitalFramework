#pragma once

#include <DFW/Modules/ECS/System.h>
#include <DFW/Modules/ECS/Managers/EntityRegistry.h>

namespace DFW
{
    class TransformSystem : public DECS::System::Registrar<TransformSystem>
    {
    public:
        TransformSystem() = default;
        virtual ~TransformSystem() = default;

    private:
        virtual void Init(DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DECS::EntityRegistry& a_registry) override;

        virtual void Update(DECS::EntityRegistry& a_registry) override;

    };

} // End of namespace ~ DFW.
