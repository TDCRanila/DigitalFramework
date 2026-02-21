#pragma once

#include <DFW/GameWorld/Controller/ControllerNameID.h>
#include <DFW/GameWorld/Controller/BaseController.h>

#include <DFW/Modules/ECS/System.h>

#include <DFW/CoreSystems/Memory.h>
#include <DFW/CoreSystems/Logging/Logger.h>

#include <unordered_map>

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

        template <typename ControllerType>
        SharedPtr<ControllerType> ConstructController(ControllerNameID const& a_controller_name_id);

        template <typename ControllerType = BaseController>
        SharedPtr<ControllerType> GetController(ControllerNameID const& a_controller_name_id);

    protected:
        virtual void Update(DECS::EntityRegistry& a_registry) override;

    protected:
        std::unordered_map<ControllerNameID, SharedPtr<BaseController>> _controllers;

    };

#pragma region Template Implementation

    template <class ControllerType>
    SharedPtr<ControllerType> ControllerSystem::ConstructController(ControllerNameID const& a_controller_name_id)
    {
        SharedPtr<ControllerType> const controller = DFW::MakeShared<ControllerType>();
        controller->_name_id = a_controller_name_id;
        
        _controllers.emplace(a_controller_name_id, controller);
        
        return controller;
    }

    template <typename ControllerType>
    SharedPtr<ControllerType> ControllerSystem::GetController(ControllerNameID const& a_controller_name_id)
    {
        auto const& it = _controllers.find(a_controller_name_id);
        if (it == _controllers.end())
        {
            DFW_WARNLOG("Cannot find registered controller with name id: {} ", a_controller_name_id);
            return DFW::SharedPtr<ControllerType>();
        }

        if constexpr (not AreSameTypes<BaseController, ControllerType>)
            return std::dynamic_pointer_cast<ControllerType>(it->second);
        else
            return it->second;
    }

#pragma endregion

} // End of namespace ~ DFW.
