#include <CoreSystems/ApplicationInstance.h>

#include <CoreSystems/CoreServices.h>

#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>

#include <CoreSystems/Events/EventLibrary.h>
// TODO REMOVE - Replace with some sort of EZ Use for Input
#include <CoreSystems/Logging/Logger.h>
#include <Defines/InputDefines.h>

#include <array>

namespace DFW
{    
    ApplicationInstance::ApplicationInstance()
        : _stage_stack_communicator(nullptr)
        , _application_name("")
        , _game_clock_log_timer(true)
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

        // Core Services
        CoreService::ProvideGameClock(&_game_clock);
        CoreService::ProvideECS(&_ecs_module);
        CoreService::ProvideInputSystem(&_input_system);
        CoreService::ProvideWindowSystem(&_window_management);

        // Application
        TimeTracker application_timer;
        DFW_INFOLOG("{} - Init Application.", _application_name);
        application_timer.StartTimer();
        InitApplication();
        const TimeUnit elapsed_init_time = application_timer.ResetAndFetchElapsedTime(false);
        DFW_INFOLOG("{} - Init Application Complete - Elapsed Time: {}", _application_name, elapsed_init_time);

        DFW_INFOLOG("{} - Running Application.", _application_name);
        UpdateApplication();
        
        DFW_INFOLOG("{} - Terminating Application.", _application_name);
        application_timer.StartTimer();
        TerminateApplication();
        const TimeUnit elapsed_termination_time = application_timer.ResetAndFetchElapsedTime(false);
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
        
        // Event Library
        DFW::EventLibrary::ProcessEventCollection<DFW::StageEvent>();

        // DECS
        _ecs_module.InitECS();

        // Stage Communicator
        if (_stage_stack_communicator == nullptr)
            DFW_WARNLOG("Potentially not registering a stage stack communicator before application load. This might cause issues.");

        // Window Management
        _window_management.ChangeDefaultWindowName(_application_name);
        _window_management.BindApplicationEventFunc(DFW_BIND_FUNC(ApplicationInstance::OnApplicationEvent));
        _window_management.InitWindowManagement();

        // Imgui
        DUID const window_id = _window_management.GetMainWindow();
        DWindow::WindowInstance const& window_instance = _window_management._window_instances.at(window_id);
        _imgui.InitImGuiLayer(window_instance);

        // Gfx
        bgfx::setDebug(BGFX_DEBUG_TEXT);

        // User-Implemented Post-Initialisation.
        PostApplicationInit();
    }

    void ApplicationInstance::UpdateApplication()
    {         
        bool should_run(true);
        while (should_run)
        {
            // Clock
            float32 game_clock_log_interval(10.0f);
            if (_game_clock_log_timer.FetchElapsedTime() > game_clock_log_interval)
            {
                _game_clock_log_timer.ResetTimer(true);
                _game_clock.DebugLog();
            }

            _game_clock.BeginGameFrame();
            
            glfwPollEvents();

            // Input
            _input_system.ProcessInputEvents();

            // Update Game Instance(s)
            if (_window_management.HaveAllWindowsBeenClosed())
            {
                should_run = false;
            }
            else
            {
                Debug_DrawBGFXInfo();

                // Main
                std::vector<StageBase*> const& _stages = _stage_stack_controller.GetStages();
                for (StageBase* stage : _stages)
                {
                    if (!stage->IsDisabled())
                        stage->Update();
                }

                _ecs_module.UpdateECS();

                // Render - imgui
                _imgui.BeginFrame(_game_clock.GetLastFrameDeltaTime());

                for (StageBase* stage : _stages)
                {
                    if (!stage->IsDisabled())
                        stage->RenderImGui();
                }

                _imgui.EndFrame();

                // Render - bgfx
                bgfx::ViewId main_window = 0;
                bgfx::setViewRect(main_window, 0, 0, bgfx::BackbufferRatio::Equal);
                bgfx::setViewClear(main_window, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x33333333);

                // Advance to next frame. Process submitted rendering primitives.
                bgfx::frame();
            }

            _game_clock.EndGameFrame();
        }
    }

    void ApplicationInstance::TerminateApplication()
    {
        // Gracefully remove attached stages.
        _stage_stack_controller.RemoveAllAttachedStages();
        _stage_stack_controller.DeleteAllAttachedStages();

        _window_management.TerminateWindowManagement();

        _ecs_module.TerminateECS();

        CoreService::ProvideGameClock(nullptr);
        CoreService::ProvideECS(nullptr);
        CoreService::ProvideInputSystem(nullptr);
        CoreService::ProvideWindowSystem(nullptr);
    }

    void ApplicationInstance::OnApplicationEvent(ApplicationEvent& a_event)
    {
        DFW_LOG("ApplicationEvent Received: {}", a_event.GetDebugString());

        const std::vector<StageBase*>& _stages = _stage_stack_controller.GetStages();
        for (auto stage_it = _stages.rbegin(); stage_it != _stages.rend(); ++stage_it)
        {
            StageBase* stage_ptr = (*stage_it);
            if (!stage_ptr->IsDisabled())
                stage_ptr->OnApplicationEvent(a_event);
        }
    }

    void ApplicationInstance::Debug_DrawBGFXInfo() const
    {
        DUID const window_id = _window_management.GetMainWindow();
        DWindow::WindowDimension const& window_dimension = _window_management._window_instances.at(window_id)._window_dimension;

        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        bgfx::touch(0);

        // bgfx debug Window Information
        bgfx::dbgTextClear();

        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to hide debug information. Press Shift+F1 to toggle stats.");

        bgfx::dbgTextPrintf(0, 1, 0x0f, "Digital Info: ");

        std::string is_window_focussed_string = _window_management.IsWindowFocussed(window_id) ? "Window is focussed." : "Window is unfocussed.";
        bgfx::dbgTextPrintf(15, 1, 0x0f, is_window_focussed_string.c_str());

        bgfx::dbgTextPrintf(38, 1, 0x0f, "fW:%d x fH:%d.", window_dimension._current_frame_width, window_dimension._current_frame_height);
        bgfx::dbgTextPrintf(58, 1, 0x0f, "W:%d x H:%d.", window_dimension._current_width, window_dimension._current_height);

        const bgfx::Stats* stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 2, 0x0f, "Bgfx backbuffer %dW x %dH in px, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

        bgfx::dbgTextPrintf(0, 3, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(0, 4, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(0, 5, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

        // Enable stats or debug text.
        static bool show_stats = false;
        static bool show_debug_info = true;
        static uint32 bgfx_debug = BGFX_DEBUG_TEXT;

        bool key_f1_pressed = _input_system.IsKeyReleased(DInput::DKey::F1);
        if (key_f1_pressed)
        {
            bool key_shift_pressed = _input_system.IsKeyDown(DInput::DKey::LEFT_SHIFT) || _input_system.IsKeyDown(DInput::DKey::RIGHT_SHIFT);
            if (show_debug_info && key_shift_pressed)
            {
                bgfx_debug = show_stats ? BGFX_DEBUG_TEXT : BGFX_DEBUG_STATS;
                show_stats = show_stats ? false : true;
                bgfx::setDebug(bgfx_debug);
            }
            else if (!key_shift_pressed)
            {
                bgfx::setDebug(show_debug_info ? BGFX_DEBUG_NONE : bgfx_debug);
                show_debug_info = show_debug_info ? false : true;
            }
        }
    }

} // End of namespace ~ DFW.
