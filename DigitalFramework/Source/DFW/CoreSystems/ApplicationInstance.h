#pragma once

#include <DFW/CoreSystems/ApplicationEvents.h>
#include <DFW/CoreSystems/Events/EventDispatcher.h>
#include <DFW/CoreSystems/GameClock.h>
#include <DFW/CoreSystems/TimeTracker.h>
#include <DFW/CoreSystems/Stage/StageStackController.h>
#include <DFW/CoreSystems/Stage/StageStackCommunicator.h>

namespace DFW
{

#pragma region FW Declares.
    class ImGuiLayer;

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

#pragma endregion

    class ApplicationInstance
    {
    public:
        ApplicationInstance();
        virtual ~ApplicationInstance();

        void RunApplication(char const* a_name);
        void RunApplication(std::string const& a_name);

    protected:
        virtual void PreApplicationInit() = 0;
        virtual void PostApplicationInit() = 0;

    private:
        void InitApplication();
        void TerminateApplication();
        void UpdateApplication();

        SharedPtr<GameClock> _game_clock;
        SharedPtr<EventDispatcher> _application_event_handler;
        SharedPtr<StageStackController> _stage_controller;

        SharedPtr<DWindow::WindowManagement> _window_management;
        SharedPtr<DInput::InputManagementSystem> _input_management;
        SharedPtr<DRender::RenderModule> _render_module;
        SharedPtr<DResource::ResourceManager> _resource_manager;
 
        UniquePtr<ImGuiLayer> _imgui;
 
        std::string _application_name;
        bool _should_application_run;
        bool _should_application_start_or_reset;

    private:
        void OnApplicationCloseEvent(ApplicationCloseEvent& a_event);
        void OnApplicationResetEvent(ApplicationResetEvent& a_event);

        bool Debug_CheckForEmergencyApplicationExit() const;
        void Debug_ReportGameClockInfo(DFW::TimeUnit const a_log_interval) const;
        void Debug_ToggleMouseCursorCapture() const;
    };

} // End of namespace ~ DFW.

#define DFW_APP_ENTRY(a_name, a_app_class)      \
    int main(int /*argc*/, char** /*argv*/)     \
    {                                           \
    a_app_class app;                            \
    app.RunApplication(#a_name);                \
    }                                           \
