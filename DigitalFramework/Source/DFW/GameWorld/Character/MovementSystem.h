#pragma once

#include <DFW/Modules/ECS/System.h>

namespace DFW
{
    class MovementSystem : public DECS::System::Registrar<MovementSystem>
    {
    public:
        MovementSystem() = default;
        virtual ~MovementSystem() = default;

    private:
        virtual void Update(DECS::EntityRegistry& a_registry) override;
        virtual void PostUpdate(DECS::EntityRegistry& a_registry) override;

    };

} // End of namespace ~ DFW.
