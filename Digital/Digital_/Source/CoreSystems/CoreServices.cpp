#include <CoreSystems/CoreServices.h>

#include <Utility/TemplateUtility.h>

#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
    WeakPtr<GameClock> CoreService::_gameclock_service;
    WeakPtr<EventDispatcher> CoreService::_app_eventhandler_service;
    WeakPtr<StageStackController> CoreService::_app_stagecontroller_service;

    WeakPtr<DWindow::WindowManagement> CoreService::_window_management_service;
    WeakPtr<DInput::InputManagementSystem> CoreService::_input_management_service;
    WeakPtr<DRender::RenderModule> CoreService::_render_module_service;
    WeakPtr<DResource::ResourceManager> CoreService::_resource_manager_service;

    namespace Detail
    {
        char const* invalid_provided_service_message("No or invalid Service provided.");
        std::string const provided_service_message("Service has been provided; ");

    } // End of namespace ~ Detail.

    void CoreService::ReleaseServices()
    {
        _gameclock_service.reset();
        _app_eventhandler_service.reset();
        _app_stagecontroller_service.reset();

        _window_management_service.reset();
        _input_management_service.reset();
        _render_module_service.reset();
        _resource_manager_service.reset();

        DFW_INFOLOG("All services provided have been released.");
    }

    SharedPtr<GameClock> CoreService::GetGameClock()
    {
        DFW_ASSERT(!_gameclock_service.expired() && Detail::invalid_provided_service_message);
        return _gameclock_service.lock();
    }

    void CoreService::ProvideGameClock(SharedPtr<GameClock> const& a_provided_service)
    {
        _gameclock_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Game Clock"));
    }

    SharedPtr<EventDispatcher> CoreService::GetAppEventHandler()
    {
        DFW_ASSERT(!_app_eventhandler_service.expired() && Detail::invalid_provided_service_message);
        return _app_eventhandler_service.lock();
    }

    void CoreService::ProvideAppEventHandler(SharedPtr<EventDispatcher> const& a_provided_service)
    {
        _app_eventhandler_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Application Event Handler"));
    }

    SharedPtr<StageStackController> CoreService::GetAppStageController()
    {
        DFW_ASSERT(!_app_stagecontroller_service.expired() && Detail::invalid_provided_service_message);
        return _app_stagecontroller_service.lock();
    }

    void CoreService::ProvideAppStageController(SharedPtr<StageStackController> const& a_provided_service)
    {
        _app_stagecontroller_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Application Stack Controller"));
    }

    SharedPtr<DWindow::WindowManagement> CoreService::GetWindowManagement()
    {
        DFW_ASSERT(!_window_management_service.expired() && Detail::invalid_provided_service_message);
        return _window_management_service.lock();
    }

    void CoreService::ProvideWindowManagementService(SharedPtr<DWindow::WindowManagement> const& a_provided_service)
    {
        _window_management_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Window Management"));
    }

    SharedPtr<DInput::InputManagementSystem> CoreService::GetInputManagement()
    {
        DFW_ASSERT(!_input_management_service.expired() && Detail::invalid_provided_service_message);
        return _input_management_service.lock();
    }

    void CoreService::ProvideInputManagementService(SharedPtr<DInput::InputManagementSystem> const& a_provided_service)
    {
        _input_management_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Input Management"));
    }

    SharedPtr<DRender::RenderModule> CoreService::GetRenderModule()
    {
        DFW_ASSERT(!_render_module_service.expired() && Detail::invalid_provided_service_message);
        return _render_module_service.lock();
    }

    void CoreService::ProvideRenderModule(SharedPtr<DRender::RenderModule> const& a_provided_service)
    {
        _render_module_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Render Module"));
    }

    SharedPtr<DResource::ResourceManager> CoreService::GetResourceManager()
    {
        DFW_ASSERT(!_resource_manager_service.expired() && Detail::invalid_provided_service_message);
        return _resource_manager_service.lock();
    }

    void CoreService::ProvideResourceManager(SharedPtr<DResource::ResourceManager> const& a_provided_service)
    {
        _resource_manager_service = a_provided_service;
        DFW_INFOLOG((Detail::provided_service_message + "Resource Manager"));
    }

} // End of namespace ~ DFW.
