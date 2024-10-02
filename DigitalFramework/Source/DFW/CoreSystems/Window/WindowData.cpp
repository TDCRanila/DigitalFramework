#include <DFW/CoreSystems/Window/WindowData.h>

#include <DFW/CoreSystems/Window/WindowManagement.h>

namespace DFW
{
    namespace DWindow
    {
        WindowParameters::WindowParameters()
            : name(DFW_DEFAULT_WINDOW_NAME)
            , width(DFW_DEFAULT_WINDOW_WIDTH)
            , height(DFW_DEFAULT_WINDOW_HEIGHT)
        {
        }

        WindowParameters::WindowParameters(std::string const& a_window_name, int32 const a_window_width, int32 const a_window_height)
            : name(a_window_name)
            , width(a_window_width)
            , height(a_window_height)
        {
        }

        WindowDimension::WindowDimension()
            : _current_width(DFW_DEFAULT_WINDOW_WIDTH)
            , _current_height(DFW_DEFAULT_WINDOW_HEIGHT)
            , _current_frame_width(DFW_DEFAULT_WINDOW_WIDTH)
            , _current_frame_height(DFW_DEFAULT_WINDOW_HEIGHT)
            , _current_x_pos(32)
            , _current_y_pos(32)
            , _window_frame_left(0)
            , _window_frame_top(0)
            , _window_frame_right(0)
            , _window_frame_bottom(0)
        {
        }

        WindowInstance::WindowInstance()
            : _id(DFW::DFW_INVALID_DUID)
            , _name("")
            , _should_be_closed(false)
            , is_minimized(false)
            , is_focussed(false)
        {
        }
    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
