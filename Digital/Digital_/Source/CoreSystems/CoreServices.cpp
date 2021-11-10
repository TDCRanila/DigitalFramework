#include <CoreSystems/CoreServices.h>

#include <Utility/TemplateUtility.h>

namespace DFW
{
    GameClock* CoreService::_gc_service                         = nullptr;
    DECS::ECSModule* CoreService::_ecs_service                  = nullptr;
    DInput::InputManagementSystem* CoreService::_input_service  = nullptr;

    // TODO: Could provide some game clock null-service instead.
    // maybe not needed for a gameclock but other services such as audio
    // could be useful.

    GameClock* CoreService::GetGameClock()
    {
        DFW_ASSERT(_gc_service && "No Game Clock provided.");
        return _gc_service;
    }

    void CoreService::ProvideGameClock(GameClock* a_provided_service)
    {
        _gc_service = a_provided_service;
    }

    DECS::ECSModule* CoreService::GetECS()
    {
        DFW_ASSERT(_ecs_service && "No DECS Module provided.");
        return _ecs_service;
    }

    void CoreService::ProvideECS(DECS::ECSModule* a_provided_service)
    {
        _ecs_service = a_provided_service;
    }

    DInput::InputManagementSystem* CoreService::GetInputSystem()
    {
        DFW_ASSERT(_input_service && "No Input Management System provided.");
        return _input_service;
    }

    void CoreService::ProvideInputSystem(DInput::InputManagementSystem* a_provided_service)
    {
        _input_service = a_provided_service;
    }

} // End of namespace ~ DFW.
