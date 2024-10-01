#pragma once

namespace DFW
{
    // FW Declare.
    class GameClock;
    class EventDispatcher;
    class StageStackController;

    namespace DInput
    {
        class InputManagementSystem;
    } // End of namespace ~ DInput.
    
    namespace DWindow
    {
        class WindowManagement;
    } // End of namespace ~ DWindow.

    namespace DRender
    {
        class RenderModule;
    } // End of namespace ~ DRender.

    namespace DResource
    {
        class ResourceManager;
    } // End of namespace ~ DResource.

    class CoreService
    {
    public:
        static void ReleaseServices();

        static SharedPtr<GameClock> GetGameClock();
        static SharedPtr<EventDispatcher> GetAppEventHandler();
        static SharedPtr<StageStackController> GetAppStageController();

        static SharedPtr<DWindow::WindowManagement> GetWindowManagement();
        static SharedPtr<DInput::InputManagementSystem> GetInputManagement();
        static SharedPtr<DRender::RenderModule> GetRenderModule();
        static SharedPtr<DResource::ResourceManager> GetResourceManager();

    private:
        friend class ApplicationInstance;

        static void ProvideGameClock(SharedPtr<GameClock> const& a_provided_service);
        static void ProvideAppEventHandler(SharedPtr<EventDispatcher> const& a_provided_service);
        static void ProvideAppStageController(SharedPtr<StageStackController> const& a_provided_service);

        static void ProvideWindowManagementService(SharedPtr<DWindow::WindowManagement> const& a_provided_service);
        static void ProvideInputManagementService(SharedPtr<DInput::InputManagementSystem> const& a_provided_service);
        static void ProvideRenderModule(SharedPtr<DRender::RenderModule> const& a_provided_service);
        static void ProvideResourceManager(SharedPtr<DResource::ResourceManager> const& a_provided_service);
        
        static WeakPtr<GameClock> _gameclock_service;      
        static WeakPtr<EventDispatcher> _app_eventhandler_service;
        static WeakPtr<StageStackController> _app_stagecontroller_service;

        static WeakPtr<DWindow::WindowManagement> _window_management_service;
        static WeakPtr<DInput::InputManagementSystem> _input_management_service;
        static WeakPtr<DRender::RenderModule> _render_module_service;
        static WeakPtr<DResource::ResourceManager> _resource_manager_service;
        
    };

} // End of namespace ~ DFW.
