#include <CoreSystems/ApplicationInstance.h>

#include <CoreSystems/CoreServices.h>

#include <CoreSystems/Events/EventLibrary.h>
#include <CoreSystems/Events/EventImplementation.h>
// TODO REMOVE - Replace with some sort of EZ Use for Input
#include <CoreSystems/Logging/Logger.h>
#include <Defines/InputDefines.h>

#include <Modules/Rendering/RenderModule_Impl_BGFX.h>

#include <array>

namespace DFW
{    
    ApplicationInstance::ApplicationInstance()
        : _stage_stack_communicator(nullptr)
        , _application_name("")
        , _game_clock(0.0f, false)
    {
    }

    ApplicationInstance::~ApplicationInstance()
    {
    }

    void ApplicationInstance::RunApplication(std::string const& a_name)
    {
        RunApplication(a_name.c_str());
    }

    void ApplicationInstance::RunApplication(const char* a_name)
    {
        _application_name = a_name;

        // Logging
        Logger::Init(true, 5000);

        // Allocate Systems
        _window_management  = DWindow::WindowManagementSystem::Construct();
        _render_module      = MakeUnique<DRender::RenderModuleBGFX>();

        // Core Services
        CoreService::ProvideGameClock(&_game_clock);
        CoreService::ProvideMainEventHandler(&application_event_handler);
        CoreService::ProvideECS(&_ecs_module);
        CoreService::ProvideInputSystem(&_input_system);
        CoreService::ProvideWindowSystem(_window_management.get());

        // Application
        TimeTracker application_timer(false);
        DFW_INFOLOG("{} - Init Application.", _application_name);
        application_timer.StartTimer();
        InitApplication();
        TimeUnit const elapsed_init_time = application_timer.ResetAndFetchElapsedTime(false);
        DFW_INFOLOG("{} - Init Application Complete - Elapsed Time: {}", _application_name, elapsed_init_time);

        DFW_INFOLOG("{} - Running Application.", _application_name);
        UpdateApplication();
        
        DFW_INFOLOG("{} - Terminating Application.", _application_name);
        application_timer.StartTimer();
        TerminateApplication();
        TimeUnit const elapsed_termination_time = application_timer.ResetAndFetchElapsedTime(false);
        DFW_INFOLOG("{} - Terminating Application Complete - Elapsed Time: {}", _application_name, elapsed_termination_time);
    }

    StageStackController& ApplicationInstance::ProvideStageStackController()
    {
        return _stage_stack_controller;
    }

    void ApplicationInstance::RegisterStageStackCommunicator(std::shared_ptr<StageStackCommunicator> a_stack_communicator)
    {
        if (auto ptr = a_stack_communicator.get())
        {
            _stage_stack_communicator = a_stack_communicator;

            _stage_stack_communicator->SetStageStackController(_stage_stack_controller);
            _stage_stack_controller.SetStageStackCommunicator(_stage_stack_communicator);
        }
    }

    void ApplicationInstance::PreApplicationInit()
    {   
    }

    void ApplicationInstance::PostApplicationInit()
    {
    }

    void ApplicationInstance::InitApplication()
    {
        // User-Implemented Pre-Initialisation.
        PreApplicationInit();
        
        // Stage Communicator
        if (_stage_stack_communicator == nullptr)
            DFW_WARNLOG("Potentially not registering a stage stack communicator before application load. This might cause issues.");

        // Event Library
        DFW::EventLibrary::ProcessEventCollection<DFW::StageEvent>();

        // Window Management
        _window_management->InitWindowManagement();
        DWindow::WindowParameters param(_application_name, DWindow::DFW_DEFAULT_WINDOW_WIDTH, DWindow::DFW_DEFAULT_WINDOW_HEIGHT);
        _window_management->ChangeWindowParameters(_window_management->GetMainWindowID(), param);

        // Input Management
        _input_system.InitInputManagement();

        // RenderModule
        _render_module->InitRenderModule();       

        // ImGui
        _imgui.InitImGuiLayer();

        // DECS
        _ecs_module.InitECS();

        // OS focusses on the main window. Make sure systems know about it.
        application_event_handler.InstantBroadcast<WindowFocusEvent>(_window_management->GetMainWindowID(), true);

        // User-Implemented Post-Initialisation.
        PostApplicationInit();
    }

    void ApplicationInstance::TerminateApplication()
    {
        // DECS
        _ecs_module.TerminateECS();

        // ImGui
        _imgui.TerminateImGuiLayer();

        // Render
        _render_module->TerminateRenderModule();

        // Input
        _input_system.TerminateInputManagement();

        // Window Management
        _window_management->TerminateWindowManagement();

        // Gracefully remove attached stages.
        _stage_stack_controller.RemoveAllAttachedStages();
        _stage_stack_controller.DeleteAllAttachedStages();

        CoreService::ProvideGameClock(nullptr);
        CoreService::ProvideECS(nullptr);
        CoreService::ProvideInputSystem(nullptr);
        CoreService::ProvideWindowSystem(nullptr);
    }

    void ApplicationInstance::UpdateApplication()
    {         
        bool should_run(true);
        while (should_run)
        {
            _game_clock.BeginGameFrame();
            
            _window_management->PollWindowEvents();
            _input_system.ProcessInputEvents();

            // Update Game Instance(s)
            if (_window_management->HaveAllWindowsBeenClosed())
            {
                should_run = false;
            }
            else
            {               
                std::vector<StageBase*> const& _stages = _stage_stack_controller.GetStages();
                { // Regular Application Update
                    _render_module->BeginFrame();

                    // Update Stages.
                    for (StageBase* stage : _stages)
                    {
                        if (!stage->IsDisabled())
                            stage->Update();
                    }

                    _ecs_module.UpdateECS();

                    _render_module->EndFrame();
                }

                { // ImGui Related Update.
                    _imgui.BeginFrame(_game_clock.GetLastFrameDeltaTime());

                    for (StageBase* stage : _stages)
                    {
                        if (!stage->IsDisabled())
                            stage->RenderImGui();
                    }

                    _ecs_module.UpdateECSImGui();

                    _imgui.EndFrame();
                }

                // Render all the submitted items.
                _render_module->RenderFrame();
                _render_module->Debug_DrawBasicRenderInfo();
            }

            _game_clock.EndGameFrame();

            Debug_ReportGameClockInfo(DFW::TimeUnit(10.f));
        }
    }

    void ApplicationInstance::Debug_ReportGameClockInfo(DFW::TimeUnit const a_log_interval)
    {
        static TimeTracker game_clock_log_timer(true);
        if (game_clock_log_timer.FetchElapsedTime() > a_log_interval)
        {
            game_clock_log_timer.ResetTimer(true);
            _game_clock.Debug_LogInfo();
        }
    }

} // End of namespace ~ DFW.
