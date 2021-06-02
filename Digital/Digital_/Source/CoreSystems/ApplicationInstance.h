#pragma once

#include <CoreSystems/GameClock.h>
#include <CoreSystems/TimeTracker.h>
#include <CoreSystems/InputManagement.h>
#include <CoreSystems/Events/EventImplementation.h>
#include <CoreSystems/Stage/StageStackController.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <CoreSystems/ImGui/ImGuiLayer.h>

namespace DCore
{
    // FW Declare.
    class StageStackCommunicator;

    class ApplicationInstance
    {
    public:
        ApplicationInstance();
        virtual ~ApplicationInstance();

        void RunApplication(const char* a_name);
        void RunApplication(const std::string a_name = "DIGITAL");

        static WindowManagementSystem* ProvideWindowManagement();
        static InputManagementSystem* ProvideInputManagment();

    protected:

        void RegisterStackCommunicator(std::shared_ptr<StageStackCommunicator> a_stack_communicator);
        StageStackController& ProvideStackController();

        virtual void PreApplicationLoad();
        virtual void PostApplicationLoad();

    private:
        void InitApplication();
        void ApplicationLoad();
        void UpdateApplication();
        void TerminateApplication();

        void OnApplicationEvent(BaseEvent& a_event);

        static WindowManagementSystem _window_management;
        static InputManagementSystem _input_management;
        
        ImGuiLayer _imgui;
 
        TimeTracker _game_clock_log_timer;
        GameClock _game_clock;

        StageStackController _stage_stack_controller;
        std::shared_ptr<StageStackCommunicator> _stage_stack_communicator;

        std::string _application_name;
    };

} // End of namespace ~ DCore

#define DFW_APP_ENTRY(a_name, a_app_class)      \
    int main(int /*argc*/, char** /*argv*/)     \
    {                                           \
    a_app_class app;                            \
    app.RunApplication(#a_name);                \
    }                                           \

 
