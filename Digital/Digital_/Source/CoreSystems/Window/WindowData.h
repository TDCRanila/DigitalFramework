#pragma once

#include <CoreSystems/Window/WindowID.h>

#include <CoreSystems/ApplicationEvents.h>

// Forward Declare(s)
struct GLFWwindow;

namespace DFW
{
    // Forward Declare(s)
    namespace DInput
    {
        struct InputData;
    }

    namespace DWindow
    {
        class WindowManagementSystem;
        
        struct WindowParameters
        {
            WindowParameters();
            WindowParameters(std::string const& a_window_name, int32 const a_window_width, int32 const a_window_height);

            std::string name;
            int32       width;
            int32       height;
        };

        struct WindowDimension
        {
            WindowDimension();

            int32 _current_frame_width, _current_frame_height;
            int32 _current_width, _current_height;
            int32 _current_x_pos, _current_y_pos; // Point is the the upper-left corner.
            int32 _window_frame_left, _window_frame_top, _window_frame_right, _window_frame_bottom;
        };

        struct WindowInstance
        {
            WindowInstance();

            WindowID        _id;
            std::string     _name;
            
            WindowDimension _window_dimension;
            DInput::InputData* _input_data;

            bool _should_be_closed;
            bool _is_minimized;
            bool _is_focussed;

            ApplicationEventCallbackFunc _application_event_callback_func;

        };

    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
