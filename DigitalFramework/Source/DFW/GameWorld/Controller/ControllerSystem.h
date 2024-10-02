#pragma once

#include <GameWorld/Controller/CharacterController.h>

#include <Modules/ECS/System.h>

#include <CoreSystems/Memory.h>

#include <vector>

namespace DFW
{
    namespace DECS
    {
        class EntityRegistry;
    } // End of namespace ~ DECS.

    class ControllerSystem : public DECS::System::Registrar<ControllerSystem>
    {
    public:
        ControllerSystem() = default;
        virtual ~ControllerSystem() = default;

        void RegisterController(SharedPtr<CharacterController> const& a_controller) { _controllers.emplace_back(a_controller); }

    private:
        virtual void Update(DECS::EntityRegistry& a_registry) override;

    private:
        std::vector<SharedPtr<CharacterController>> _controllers;

    };

} // End of namespace ~ DFW.
