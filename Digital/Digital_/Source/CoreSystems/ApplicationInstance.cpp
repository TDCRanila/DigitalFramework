#include <CoreSystems/ApplicationInstance.h>

#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>

// TODO REMOVE - Replace with some sort of EZ Use for Input
#include <CoreSystems/Logging/Logger.h>
#include <Defines/InputDefines.h>

#include <array>

namespace DCore
{    
    WindowManagementSystem ApplicationInstance::_window_management;
    InputManagementSystem ApplicationInstance::_input_management;

    ApplicationInstance::ApplicationInstance()
        : _stage_stack_communicator(nullptr)
        , _application_name("")
    {
        _stage_stack_controller._stack_communicator = _stage_stack_communicator;
    }

    ApplicationInstance::~ApplicationInstance()
    {
    }

    void ApplicationInstance::RegisterStackCommunicator(std::shared_ptr<StageStackCommunicator> a_stack_communicator)
    {
        _stage_stack_communicator = a_stack_communicator;
    }

    void ApplicationInstance::RunApplication(const std::string a_name)
    {
        RunApplication(a_name.c_str());
    }

    void ApplicationInstance::RunApplication(const char* a_name)
    {
        _application_name = a_name;

        Logger::Init(true, 5000);

        TimeTracker application_timer;
        DFW_INFOLOG("{} - Init Application.", _application_name);
        application_timer.StartTimer();
        InitApplication();
        const TimeUnit elapsed_init_time = application_timer.ResetAndFetchTime(false);
        DFW_INFOLOG("{} - Init Application Complete - Elapsed Time: {}", _application_name, elapsed_init_time);

        DFW_INFOLOG("{} - Running Application.", _application_name);
        UpdateApplication();
        
        DFW_INFOLOG("{} - Terminating Application.", _application_name);
        application_timer.StartTimer();
        TerminateApplication();
        const TimeUnit elapsed_termination_time = application_timer.ResetAndFetchTime(false);
        DFW_INFOLOG("{} - Terminating Application Complete - Elapsed Time: {}", _application_name, elapsed_termination_time);
    }

    WindowManagementSystem* ApplicationInstance::ProvideWindowManagement()
    {
        return &_window_management;
    }

    InputManagementSystem* ApplicationInstance::ProvideInputManagment()
    {
        return &_input_management;
    }

    StageStackController& ApplicationInstance::ProvideStackController()
    {
        return _stage_stack_controller;
    }

    void ApplicationInstance::PreApplicationLoad()
    {
        
    }

    void ApplicationInstance::PostApplicationLoad()
    {

    }

    void ApplicationInstance::ApplicationLoad()
    {
        _window_management.ChangeDefaultWindowName(_application_name);
        _window_management.BindApplicationEventFunc(DFW_BIND_FUNC(ApplicationInstance::OnApplicationEvent));
        _window_management.InitWindowManagement();

        const DUID window_id = _window_management.GetMainWindow();
        WindowInstance& window_instance = _window_management._window_instances.at(window_id);

        _imgui.InitImGuiLayer(window_instance);
    }

    void ApplicationInstance::InitApplication()
    {
        PreApplicationLoad();
        
        ApplicationLoad();
        bgfx::setDebug(BGFX_DEBUG_TEXT);

        PostApplicationLoad();

    }

    void ApplicationInstance::UpdateApplication()
    {
        _game_timer.StartTimer();
         
        bool should_run(true);
        while (should_run)
        {
            glfwPollEvents();
            _input_management.ProcessInputEvents();

            // Update Game Instance(s)
            if (_game_timer.FetchTime() > TimeUnit(600.f) || _window_management.HaveAllWindowsBeenClosed())
            {
                should_run = false;
            }
            else
            {
                const DUID window_id                = _window_management.GetMainWindow();
                WindowDimension& window_dimension   = _window_management._window_instances.at(window_id)._window_dimension;
                
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
                static bool show_stats      = false;
                static bool show_debug_info = true;
                static uint32 bgfx_debug    = BGFX_DEBUG_TEXT;

                bool key_f1_pressed = _input_management.IsKeyReleased(DKey::F1);
                if (key_f1_pressed)
                {
                    bool key_shift_pressed = _input_management.IsKeyDown(DKey::LEFT_SHIFT) || _input_management.IsKeyDown(DKey::RIGHT_SHIFT);
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

                // Main
                const std::vector<StageBase*>& _stages = _stage_stack_controller.GetStages();
                for (StageBase* stage : _stages)
                {
                    if (!stage->IsDisabled())
                        stage->Update();
                }

                InputData& active_input_data = _input_management._input_data_storage.at(window_id);
                _imgui.BeginFrame(active_input_data, window_dimension);

                for (StageBase* stage : _stages)
                {
                    if (!stage->IsDisabled())
                        stage->RenderImGui();
                }

                _imgui.EndFrame();

                // Render
                bgfx::ViewId main_window = 0;
                bgfx::setViewRect(main_window, 0, 0, bgfx::BackbufferRatio::Equal);
                bgfx::setViewClear(main_window, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x33333333);

                // Advance to next frame. Process submitted rendering primitives.
                bgfx::frame();
            }
        }
    }

    void ApplicationInstance::TerminateApplication()
    {
        // Gracefully remove attached stages.
        _stage_stack_controller.RemoveAttachedStages();
        _stage_stack_controller.DeleteAttachedStages();

        _window_management.TerminateWindowManagement();
    }

    void ApplicationInstance::OnApplicationEvent(BaseEvent& a_event)
    {
        DFW_INFOLOG("ApplicationEvent Received: {}", a_event.GetDebugString());

        EventDispatcher dispatcher(a_event);
        
        // TODO Send events to all stages attached to application
        const std::vector<StageBase*>& _stages = _stage_stack_controller.GetStages();
        for (auto stage_it = _stages.rbegin(); stage_it != _stages.rend(); ++stage_it)
        {
            StageBase* stage_ptr = (*stage_it);
            if (!stage_ptr->IsDisabled())
                stage_ptr->OnApplicationEvent(a_event);
        }

    }

} // End of namespace ~ DCore
