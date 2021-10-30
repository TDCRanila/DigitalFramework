#pragma once

#include <CoreSystems/GameClock.h>
#include <CoreSystems/TimeTracker.h>
#include <CoreSystems/InputManagement.h>
#include <CoreSystems/Events/EventImplementation.h>
#include <CoreSystems/Stage/StageStackController.h>
#include <CoreSystems/Stage/StageStackCommunicator.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <Modules/ECS/ECSModule.h>

#include <CoreSystems/ImGui/ImGuiLayer.h>

namespace DCore
{
    class ApplicationInstance
    {
    public:
        ApplicationInstance();
        virtual ~ApplicationInstance();

        void RunApplication(char const* a_name);
        void RunApplication(std::string const& a_name);

        static WindowManagementSystem* ProvideWindowManagement();
        static InputManagementSystem* ProvideInputManagment();

    protected:
        void RegisterStageStackCommunicator(std::shared_ptr<StageStackCommunicator> a_stack_communicator);

        StageStackController& ProvideStageStackController();

        virtual void PreApplicationInit();
        virtual void PostApplicationInit();

    private:
        void InitApplication();
        void UpdateApplication();
        void TerminateApplication();

        void OnApplicationEvent(ApplicationEvent& a_event);

        void Debug_DrawBGFXInfo() const;

        static WindowManagementSystem _window_management;
        static InputManagementSystem _input_management;
        
        ImGuiLayer _imgui;
 
        TimeTracker _game_clock_log_timer;
        GameClock _game_clock;

        DECS::ECSModule _ecs_module;

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
