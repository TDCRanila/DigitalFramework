#include <CoreSystems/Window/WindowData.h>

namespace DFW
{
    WindowDimension::WindowDimension()
        : _current_width(1280)
        , _current_height(720)
        , _current_frame_width(1280)
        , _current_frame_height(720)
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
        , _window(nullptr)
        , _input_data(nullptr)
        , _should_be_closed(false)
        , _is_minimized(false)
        , _is_focussed(false)
    {
    }

} // End of namespace ~ DFW.
