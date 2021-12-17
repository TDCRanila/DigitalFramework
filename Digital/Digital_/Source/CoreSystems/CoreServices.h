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
        class WindowManagementSystem;
    } // End of namespace ~ DWindow.

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
    
        static DWindow::WindowManagementSystem* GetWindowSystem();
        static void ProvideWindowSystem(DWindow::WindowManagementSystem* a_provided_service);

    private:
        static GameClock*                       _gc_service;
        static EventDispatcher*                 _meh_service;
        static DECS::ECSModule*                 _ecs_service;
        static DInput::InputManagementSystem*   _input_service;
        static DWindow::WindowManagementSystem* _window_service;

    };

} // End of namespace ~ DFW.
