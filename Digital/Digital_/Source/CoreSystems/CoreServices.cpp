#include <CoreSystems/CoreServices.h>

#include <Utility/TemplateUtility.h>

#include <CoreSystems/Logging/Logger.h>

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

    namespace Detail
    {
        char const* invalid_provided_service_message("No or invalid Service provided.");
        std::string const provided_service_message("Service has been provided; ");

    } // End of namespace ~ Detail.

    void CoreService::ReleaseServices()
    {
        _gc_service     = nullptr;
        _meh_service    = nullptr;
        _ecs_service    = nullptr;
        _input_service  = nullptr;
        _window_service = nullptr;
        _rm_service     = nullptr;

        DFW_INFOLOG("All services provided have been released.");
    }

    GameClock* CoreService::GetGameClock()
    {
        DFW_ASSERT(_gc_service && Detail::invalid_provided_service_message);
        return _gc_service;
    }

    void CoreService::ProvideGameClock(GameClock* a_provided_service)
    {
        _gc_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Game Clock"));
    }

    EventDispatcher* CoreService::GetMainEventHandler()
    {
        DFW_ASSERT(_meh_service && Detail::invalid_provided_service_message);
        return _meh_service;
    }

    void CoreService::ProvideMainEventHandler(EventDispatcher* a_provided_service)
    {
        _meh_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Main Event Handler"));
    }

    DECS::ECSModule* CoreService::GetECS()
    {
        DFW_ASSERT(_ecs_service && Detail::invalid_provided_service_message);
        return _ecs_service;
    }

    void CoreService::ProvideECS(DECS::ECSModule* a_provided_service)
    {
        _ecs_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "ECS"));
    }

    DInput::InputManagementSystem* CoreService::GetInputSystem()
    {
        DFW_ASSERT(_input_service && Detail::invalid_provided_service_message);
        return _input_service;
    }

    void CoreService::ProvideInputSystem(DInput::InputManagementSystem* a_provided_service)
    {
        _input_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Input Management System"));
    }

    DWindow::WindowManagement* CoreService::GetWindowSystem()
    {
        DFW_ASSERT(_window_service && Detail::invalid_provided_service_message);
        return _window_service;
    }

    void CoreService::ProvideWindowSystem(DWindow::WindowManagement* a_provided_service)
    {
        _window_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Window Management System"));
    }

    DRender::RenderModule* CoreService::GetRenderModule()
    {
        DFW_ASSERT(_rm_service && Detail::invalid_provided_service_message);
        return _rm_service;
    }

    void CoreService::ProvideRenderModule(DRender::RenderModule* a_provided_service)
    {
        _rm_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Render Module"));
    }

} // End of namespace ~ DFW.
