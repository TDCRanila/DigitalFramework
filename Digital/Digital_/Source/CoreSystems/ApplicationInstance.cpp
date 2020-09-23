#include <CoreSystems/ApplicationInstance.h>

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>

#include <CoreSystems/ImGuiLayer.h>

// TODO REMOVE - Replace with some sort of EZ Use for Input
#include <Defines/InputDefines.h>

#include <array>

namespace DCore
{
    WindowManagementSystem ApplicationInstance::_window_management;
    InputManagementSystem ApplicationInstance::_input_management;

    ApplicationInstance::ApplicationInstance()
    {
    }

    ApplicationInstance::~ApplicationInstance()
    {
    }

    void ApplicationInstance::RunApplication(const std::string a_name)
    {
        RunApplication(a_name.c_str());
    }

    void ApplicationInstance::RunApplication(const char* a_name)
    {
        _application_name = a_name;

        TimeTracker application_timer;
        INFOLOG(_application_name << " - Init Application.");
        application_timer.StartTimer();
        InitApplication();
        const TimeUnit elapsed_init_time = application_timer.ResetAndFetchTime(false);
        INFOLOG(_application_name << " - Init Application Complete. - Elapsed Time: " << elapsed_init_time);

        INFOLOG(_application_name << " - Running Application.");
        UpdateApplication();
        
        INFOLOG(_application_name << " - Terminating Application.");
        application_timer.StartTimer();
        TerminateApplication();
        const TimeUnit elapsed_termination_time = application_timer.ResetAndFetchTime(false);
        INFOLOG(_application_name << " - Terminating Application Complete - Elapsed Time: " << elapsed_termination_time);
    }

    WindowManagementSystem* ApplicationInstance::ProvideWindowManagement()
    {
        return &_window_management;
    }

    InputManagementSystem* ApplicationInstance::ProvideInputManagment()
    {
        return &_input_management;
    }

    void ApplicationInstance::PreApplicationLoad()
    {
        
    }

    void ApplicationInstance::ApplicationLoad()
    {
        _window_management.ChangeDefaultWindowName(_application_name);
        _window_management.InitWindowManagement();

        imguiCreate();
    }

    void ApplicationInstance::InitApplication()
    {
        PreApplicationLoad();
        
        ApplicationLoad();

        bgfx::setDebug(BGFX_DEBUG_TEXT);
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
                const DUID window_id = _window_management.GetMainWindow();
                WindowInstance& window_instance = _window_management._window_instances.at(window_id);
                WindowDimension& window_dimension = window_instance._window_dimension;
                InputData& active_input_data = _input_management._input_data_storage.at(window_id);

                bgfx::reset(window_dimension._current_frame_width, window_dimension._current_frame_height);
                bgfx::setViewRect(0, 0, 0, uint16_t(window_dimension._current_width), uint16_t(window_dimension._current_height));

                imguiBeginFrame(active_input_data, window_dimension);

                bool* show_demo = new bool(true);
                ImGui::ShowDemoWindow(show_demo);

                imguiEndFrame();

                bgfx::touch(0);

                // Use debug font to print information about this example.
                bgfx::dbgTextClear();

                if (_window_management.IsWindowFocussed(window_id))
                    bgfx::dbgTextPrintf(0, 1, 0x0f, "Window is focussed.");
                else
                    bgfx::dbgTextPrintf(0, 1, 0x0f, "Window is unfocussed.");

                bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
                
                bgfx::dbgTextPrintf(0, 2, 0x0f, "fW:%d x fH:%d.", window_dimension._current_frame_width, window_dimension._current_frame_height);
                bgfx::dbgTextPrintf(0, 3, 0x0f, "W:%d x H:%d.", window_dimension._current_width, window_dimension._current_height);

                bgfx::dbgTextPrintf(0, 4, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
                bgfx::dbgTextPrintf(50, 3, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
                bgfx::dbgTextPrintf(50, 4, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
               
                //bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

                // Enable stats or debug text.
                static bool show_stats = false;
                if (_input_management.IsKeyReleased(DKey::F1))
                {
                    if (!show_stats)
                    {
                        show_stats = true;
                        bgfx::setDebug(BGFX_DEBUG_STATS);
                    }
                    else
                    {
                        show_stats = false;
                        bgfx::setDebug(BGFX_DEBUG_TEXT);
                    }
                    INFOLOG("Key: " << to_underlying(DKey::F1) << " is released.")
                }

                // Advance to next frame. Process submitted rendering primitives.
                bgfx::frame();
            }
        }
    }

    void ApplicationInstance::TerminateApplication()
    {
        _window_management.TerminateWindowManagement();
    }

} // End of namespace ~ DCore
