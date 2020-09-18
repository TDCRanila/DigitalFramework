#include <CoreSystems/ApplicationInstance.h>

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>

#include <CoreSystems/ImGuiLayer.h>

// TODO REMOVE - Replace with some sort of EZ Use for Input
#include <Defines/InputDefines.h>

#include <array>

namespace DCore
{
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

    void ApplicationInstance::PreApplicationLoad()
    {
        
    }

    void ApplicationInstance::ApplicationLoad()
    {
        _window_management.ChangeDefaultWindowName(_application_name);
        _window_management.ProvideInputSystem(&_input_management);
        _window_management.InitWindowManagement();

        imguiCreate();
    }

    void ApplicationInstance::InitApplication()
    {
        PreApplicationLoad();
        
        ApplicationLoad();
    }

    void ApplicationInstance::UpdateApplication()
    {
        _game_timer.StartTimer();
         
        bool should_run(true);
        while (should_run)
        {
            // Update Game Instance(s)
            if (_game_timer.FetchTime() > TimeUnit(600.f))
                should_run = false;

            glfwPollEvents();
            _input_management.ProcessInputEvents();

            InputData& active_input_data = _input_management._input_data_storage[0];
            WindowInstance& window_instance = _window_management._window_instances[0];
            WindowDimension& window_dimensions = window_instance._window_dimensions;

            bgfx::reset(window_dimensions._current_width, window_dimensions._current_height);
            bgfx::setViewRect(0, 0, 0, uint16_t(window_dimensions._current_width), uint16_t(window_dimensions._current_height));

            imguiBeginFrame(active_input_data, window_dimensions);

			bool* show_demo = new bool(true);
			ImGui::ShowDemoWindow(show_demo);

			imguiEndFrame();

            bgfx::touch(0);

            // Use debug font to print information about this example.
            bgfx::dbgTextClear();
            //bgfx::dbgTextImage(std::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, std::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
            bgfx::dbgTextPrintf(120, 0, 0x0f, "fW:%d x fH:%d.", window_dimensions._current_frame_width, window_dimensions._current_frame_height);
            bgfx::dbgTextPrintf(100, 0, 0x0f, "W:%d x H:%d.", window_dimensions._current_width, window_dimensions._current_height);

            bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
            bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
            bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
            bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
            const bgfx::Stats* stats = bgfx::getStats();
            bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
            // Enable stats or debug text.
            bgfx::setDebug(BGFX_DEBUG_STATS);

            // Advance to next frame. Process submitted rendering primitives.
            bgfx::frame();
        }
    }

    void ApplicationInstance::TerminateApplication()
    {
        _window_management.TerminateWindowManagement();
    }

} // End of namespace ~ DCore
