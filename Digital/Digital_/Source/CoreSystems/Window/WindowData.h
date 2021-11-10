#pragma once

#include <Defines/IDDefines.h>

#include <CoreSystems/ApplicationEvents.h>

// Forward Declare(s)
struct GLFWwindow;

namespace DFW
{
    // Forward Declare(s)
    struct InputData;

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

        WindowDimension _window_dimension;

        WindowID        _id;
        std::string     _name;

        GLFWwindow* _window;
        InputData* _input_data;

        bool _should_be_closed;
        bool _is_minimized;
        bool _is_focussed;

    protected:
        friend class WindowManagementSystem;

        ApplicationEventCallbackFunc _application_event_callback_func;

    };

} // End of namespace ~ DFW.
