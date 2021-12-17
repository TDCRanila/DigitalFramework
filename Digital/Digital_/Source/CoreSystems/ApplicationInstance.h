#pragma once

#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/TimeTracker.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Stage/StageStackController.h>
#include <CoreSystems/Stage/StageStackCommunicator.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <Modules/ECS/ECSModule.h>

#include <CoreSystems/ImGui/ImGuiLayer.h>

namespace DFW
{
    class ApplicationInstance
    {
    public:
        ApplicationInstance();
        virtual ~ApplicationInstance();

        void RunApplication(char const* a_name);
        void RunApplication(std::string const& a_name);

        EventDispatcher application_event_handler;

    protected:
        void RegisterStageStackCommunicator(std::shared_ptr<StageStackCommunicator> a_stack_communicator);

        StageStackController& ProvideStageStackController();

        virtual void PreApplicationInit();
        virtual void PostApplicationInit();

    private:
        void InitApplication();
        void UpdateApplication();
        void TerminateApplication();

        void Debug_DrawBGFXInfo() const;
        void Debug_ReportGameClockInfo(DFW::TimeUnit const a_log_interval);

        DWindow::WindowManagementSystem _window_management;
        DInput::InputManagementSystem _input_system;
        
        ImGuiLayer _imgui;

        GameClock _game_clock;

        DECS::ECSModule _ecs_module;

        StageStackController _stage_stack_controller;
        std::shared_ptr<StageStackCommunicator> _stage_stack_communicator;

        std::string _application_name;
    };

} // End of namespace ~ DFW.

#define DFW_APP_ENTRY(a_name, a_app_class)      \
    int main(int /*argc*/, char** /*argv*/)     \
    {                                           \
    a_app_class app;                            \
    app.RunApplication(#a_name);                \
    }                                           \
