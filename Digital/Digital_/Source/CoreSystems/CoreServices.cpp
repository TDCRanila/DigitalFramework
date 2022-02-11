#include <CoreSystems/CoreServices.h>

#include <Utility/TemplateUtility.h>

namespace DFW
{
    GameClock* CoreService::_gc_service                         = nullptr;
    EventDispatcher* CoreService::_meh_service                  = nullptr;
    DECS::ECSModule* CoreService::_ecs_service                  = nullptr;
    DInput::InputManagementSystem* CoreService::_input_service  = nullptr;
    DWindow::WindowManagement* CoreService::_window_service     = nullptr;
    DRender::RenderModule* CoreService::_rm_service             = nullptr;

    // TODO: Could provide some game clock null-service instead.
    // maybe not needed for a gameclock but other services such as audio
    // could be useful.

    GameClock* CoreService::GetGameClock()
    {
        DFW_ASSERT(_gc_service && "No or invalid Game Clock provided.");
        return _gc_service;
    }

    void CoreService::ProvideGameClock(GameClock* a_provided_service)
    {
        _gc_service = a_provided_service;
    }

    EventDispatcher* CoreService::GetMainEventHandler()
    {
        DFW_ASSERT(_gc_service && "No or invalid Game Clock provided.");
        return _meh_service;
    }

    void CoreService::ProvideMainEventHandler(EventDispatcher* a_provided_service)
    {
        _meh_service = a_provided_service;
    }

    DECS::ECSModule* CoreService::GetECS()
    {
        DFW_ASSERT(_ecs_service && "No or invalid ECS Module provided.");
        return _ecs_service;
    }

    void CoreService::ProvideECS(DECS::ECSModule* a_provided_service)
    {
        _ecs_service = a_provided_service;
    }

    DInput::InputManagementSystem* CoreService::GetInputSystem()
    {
        DFW_ASSERT(_input_service && "No or invalid Input Management System provided.");
        return _input_service;
    }

    void CoreService::ProvideInputSystem(DInput::InputManagementSystem* a_provided_service)
    {
        _input_service = a_provided_service;
    }

    DWindow::WindowManagement* CoreService::GetWindowSystem()
    {
        DFW_ASSERT(_window_service && "No or invalid Window Management System provided.");
        return _window_service;
    }

    void CoreService::ProvideWindowSystem(DWindow::WindowManagement* a_provided_service)
    {
        _window_service = a_provided_service;
    }

    DRender::RenderModule* CoreService::GetRenderModule()
    {
        DFW_ASSERT(_rm_service  && "No or invalid Render Module provided.");
        return _rm_service;
    }

    void CoreService::ProvideRenderModule(DRender::RenderModule* a_provided_service)
    {
        _rm_service = a_provided_service;
    }

} // End of namespace ~ DFW.
