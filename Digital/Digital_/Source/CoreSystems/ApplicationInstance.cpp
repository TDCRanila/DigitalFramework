#include <CoreSystems/ApplicationInstance.h>

#include <CoreSystems/CoreServices.h>

#include <CoreSystems/Events/EventLibrary.h>
#include <CoreSystems/Events/EventImplementation.h>
// TODO REMOVE - Replace with some sort of EZ Use for Input
#include <CoreSystems/Logging/Logger.h>
#include <Defines/InputDefines.h>

#include <array>

namespace DFW
{    
    ApplicationInstance::ApplicationInstance()
        : _application_name("")
    {
        // Event Library
        DFW::EventLibrary::ProcessEventCollection<DFW::StageEvent>();
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

        // Application ~ Init.
        TimeTracker application_timer(false);
        DFW_INFOLOG("{} - Init Application.", _application_name);
        application_timer.StartTimer();
        InitApplication();
        TimeUnit const elapsed_init_time = application_timer.ResetAndFetchElapsedTime(false);
        DFW_INFOLOG("{} - Init Application Complete - Elapsed Time: {}", _application_name, elapsed_init_time);

        // Application ~ Run.
        DFW_INFOLOG("{} - Running Application.", _application_name);
        UpdateApplication();
        
        // Application ~ Terminate.
        DFW_INFOLOG("{} - Terminating Application.", _application_name);
        application_timer.StartTimer();
        TerminateApplication();
        TimeUnit const elapsed_termination_time = application_timer.ResetAndFetchElapsedTime(false);
        DFW_INFOLOG("{} - Terminating Application Complete - Elapsed Time: {}", _application_name, elapsed_termination_time);
    }

    void ApplicationInstance::InitApplication()
    {
        // Allocate Services.
        _game_clock                 = MakeShared<GameClock>(0.0f, false);
        _stage_controller           = MakeShared<StageStackController>();
        _application_event_handler  = MakeShared<EventDispatcher>();
     
        _window_management          = DWindow::WindowManagement::Construct();
        _input_management           = MakeShared<DInput::InputManagementSystem>();
        _render_module              = MakeShared<DRender::RenderModule>();

        // Provide the Core Services.
        CoreService::ProvideGameClock(_game_clock);
        CoreService::ProvideAppEventHandler(_application_event_handler);
        CoreService::ProvideAppStageController(_stage_controller);

        CoreService::ProvideInputManagementService(_input_management);
        CoreService::ProvideWindowManagementService(_window_management);
        CoreService::ProvideRenderModule(_render_module);

        // User-Implemented Pre-Initialisation.
        PreApplicationInit();

        // Initialize Core Services.
        _window_management->Init();
        _window_management->ChangeWindowParameters(
                _window_management->GetMainWindowID()
            ,   DWindow::WindowParameters { _application_name, DWindow::DFW_DEFAULT_WINDOW_WIDTH, DWindow::DFW_DEFAULT_WINDOW_HEIGHT }
        );

        _input_management->Init();
        _render_module->Init();

        // Initialize ImGui Layer.
        _imgui->Init();

        // OS focusses on the main window. Make sure listeners know about this.
        _application_event_handler->InstantBroadcast<WindowFocusEvent>(_window_management->GetMainWindowID(), true);

        // User-Implemented Post-Initialisation.
        PostApplicationInit();
    }

    void ApplicationInstance::TerminateApplication()
    {
        _imgui->Terminate();

        _render_module->Terminate();
        _input_management->Terminate();
        _window_management->Terminate();

        // Gracefully remove attached stages.
        _stage_controller->RemoveAllAttachedStages();

        CoreService::ReleaseServices();
    }

    void ApplicationInstance::UpdateApplication()
    {         
        bool should_run(true);
        while (should_run)
        {
            _game_clock->BeginGameFrame();
            
            _window_management->PollWindowEvents();
            _window_management->DestroyWindowsRequestedForClosure();
            _input_management->ProcessInputEvents();
            Debug_ToggleMouseCursorCapture();

            // Update Game Instance(s)
            if (_window_management->HaveAllWindowsBeenClosed() || Debug_CheckForEmergencyApplicationExit())
            {
                should_run = false;
            }
            else
            {               
                { // Regular Application Update
                    _stage_controller->UpdateStages();

                    _render_module->BeginFrame();
                    _stage_controller->RenderStages();
                    _render_module->EndFrame();
                }

                { // ImGui Related Update.
                    _imgui->BeginFrame(_game_clock->GetLastFrameDeltaTime());
                    _stage_controller->RenderImGuiStages();
                    _imgui->EndFrame();
                }

                // Render all the submitted items.
                _render_module->RenderFrame();
                _render_module->Debug_RendererInfo();
            }

            _game_clock->EndGameFrame();

            Debug_ReportGameClockInfo(DFW::TimeUnit(10.f));
        }
    }

    bool ApplicationInstance::Debug_CheckForEmergencyApplicationExit() const
    {
        return _input_management->IsKeyDown(DFW::DInput::DKey::ESCAPE);
    }

    void ApplicationInstance::Debug_ReportGameClockInfo(DFW::TimeUnit const a_log_interval) const
    {
        static TimeTracker game_clock_log_timer(true);
        if (game_clock_log_timer.FetchElapsedTime() > a_log_interval)
        {
            game_clock_log_timer.ResetTimer(true);
            _game_clock->Debug_LogInfo();
        }
    }

    void ApplicationInstance::Debug_ToggleMouseCursorCapture() const
    {
        static bool cursor_toggle(false);
        if (_input_management->IsKeyPressed(DFW::DInput::DKey::F2))
        {
            if (cursor_toggle)
            {
                cursor_toggle = false;
                _window_management->RequestMouseCursorRelease();
            }
            else
            {
                cursor_toggle = true;
                _window_management->RequestMouseCursorCapture();
            }
        }
    }

} // End of namespace ~ DFW.
