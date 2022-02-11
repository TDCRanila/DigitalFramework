#pragma once

namespace DFW
{
    // FW Declare.
    class GameClock;
    class EventDispatcher;

    namespace DECS
    {
        class ECSModule;
    } // End of namespace ~ DECS.

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

    class CoreService
    {
    public:
        static GameClock* GetGameClock();
        static void ProvideGameClock(GameClock* a_provided_service);

        static EventDispatcher* GetMainEventHandler();
        static void ProvideMainEventHandler(EventDispatcher* a_provided_service);

        static DECS::ECSModule* GetECS();
        static void ProvideECS(DECS::ECSModule* a_provided_service);

        static DInput::InputManagementSystem* GetInputSystem();
        static void ProvideInputSystem(DInput::InputManagementSystem* a_provided_service);
    
        static DWindow::WindowManagement* GetWindowSystem();
        static void ProvideWindowSystem(DWindow::WindowManagement* a_provided_service);

        static DRender::RenderModule* GetRenderModule();
        static void ProvideRenderModule(DRender::RenderModule* a_provided_service);

    private:
        static GameClock*                       _gc_service;
        static EventDispatcher*                 _meh_service;
        static DECS::ECSModule*                 _ecs_service;
        static DInput::InputManagementSystem*   _input_service;
        static DWindow::WindowManagement*       _window_service;
        static DRender::RenderModule*           _rm_service;

    };

} // End of namespace ~ DFW.
