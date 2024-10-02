#pragma once

#include <CoreSystems/Window/WindowID.h>

namespace DFW
{
    // Forward Declare(s)
    namespace DInput
    {
        struct InputData;
    }

    namespace DWindow
    {       
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

            bool _should_be_closed;
            bool is_minimized;
            bool is_focussed;

        };

    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
