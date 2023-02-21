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

        EventDispatcher& GetEventDispatcher();
        StageStackController& GetStageStackController();

    protected:
        virtual void PreApplicationInit();
        virtual void PostApplicationInit();

    private:
        void InitApplication();
        void TerminateApplication();
        void UpdateApplication();

        bool Debug_CheckForEmergencyApplicationExit() const;
        void Debug_ReportGameClockInfo(DFW::TimeUnit const a_log_interval) const;
        void Debug_ToggleMouseCursorCapture() const;

        EventDispatcher application_event_handler;
        StageStackController _stage_stack;

        DECS::ECSModule                        _ecs_module;
        SharedPtr<DWindow::WindowManagement>    _window_management;
        DInput::InputManagementSystem           _input_system;
        DRender::RenderModule                   _render_module;
 
        ImGuiLayer _imgui;
        
        GameClock _game_clock;
 
        std::string _application_name;
    };

} // End of namespace ~ DFW.

#define DFW_APP_ENTRY(a_name, a_app_class)      \
    int main(int /*argc*/, char** /*argv*/)     \
    {                                           \
    a_app_class app;                            \
    app.RunApplication(#a_name);                \
    }                                           \
