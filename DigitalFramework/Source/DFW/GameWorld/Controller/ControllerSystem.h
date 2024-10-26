#pragma once

#include <DFW/GameWorld/Controller/BaseController.h>

#include <DFW/Modules/ECS/System.h>

#include <DFW/CoreSystems/Memory.h>

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

        void RegisterController(SharedPtr<BaseController> const& a_controller) { _controllers.emplace_back(a_controller); }

    private:
        virtual void Update(DECS::EntityRegistry& a_registry) override;

    private:
        std::vector<SharedPtr<BaseController>> _controllers;

    };

} // End of namespace ~ DFW.
