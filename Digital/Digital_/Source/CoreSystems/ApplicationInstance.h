#pragma once

#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/TimeTracker.h>
#include <CoreSystems/ImGui/ImGuiLayer.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Stage/StageStackController.h>
#include <CoreSystems/Stage/StageStackCommunicator.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/Rendering/RenderModule.h>

namespace DFW
{
    class ApplicationInstance
    {
    public:
        ApplicationInstance();
        virtual ~ApplicationInstance();

        void RunApplication(char const* a_name);
        void RunApplication(std::string const& a_name);

    protected:
        virtual void PreApplicationInit();
        virtual void PostApplicationInit();

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
 
        UniquePtr<ImGuiLayer> _imgui;
 
        std::string _application_name;

    private:
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
