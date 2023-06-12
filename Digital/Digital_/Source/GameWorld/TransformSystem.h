#pragma once

#include <Modules/ECS/System.h>
#include <Modules/ECS/Managers/EntityRegistry.h>

namespace DFW
{
    class TransformSystem : public DECS::System::Registrar<TransformSystem>
    {
    public:
        TransformSystem() = default;
        virtual ~TransformSystem() = default;

    private:
        virtual void Update(DECS::EntityRegistry& a_registry);

    };

} // End of namespace ~ DFW.
