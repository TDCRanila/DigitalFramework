#include <CoreSystems/Window/WindowManagement.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/InputManagement.h>
#include <CoreSystems/Logging/Logger.h>
#include <Defines/Defines.h>

namespace DFW
{
#pragma region GLFWWindowCallBacks
        void WindowManagementSystem::GLFWWindowCallBacks::glfw_error_callback(int error, const char* description)
        {
            UNUSED(error);
            fprintf(stderr, "GLFW Error: %s\n", description);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_window_closed_callback(GLFWwindow* a_window)
        {
            WindowInstance* user_data       = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_should_be_closed    = true;

            WindowCloseEvent event;
            user_data->_application_event_callback_func(event);

            ApplicationInstance::ProvideWindowManagement()->DestructWindow(user_data);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_window_focus_callback(GLFWwindow* a_window, int a_result)
        {
            WindowInstance* user_data   = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_is_focussed     = static_cast<bool>(a_result);

            WindowFocusEvent event(user_data->_is_focussed);
            user_data->_application_event_callback_func(event);

            if (user_data->_is_focussed)
                ApplicationInstance::ProvideWindowManagement()->SetFocussedWindowID(user_data->_id);
            else
                ApplicationInstance::ProvideWindowManagement()->SetFocussedWindowID(WindowID());
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_window_minimized_callback(GLFWwindow* a_window, int a_result)
        {
            WindowInstance* user_data   = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_is_minimized    = static_cast<bool>(a_result);

            WindowMinimizedEvent event(user_data->_is_focussed);
            user_data->_application_event_callback_func(event);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_window_position_callback(GLFWwindow* a_window, int a_x_pos, int a_y_pos)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            WindowDimension& dim = user_data->_window_dimension;
            
            const int32 old_x_pos = dim._current_x_pos;
            const int32 old_y_pos = dim._current_y_pos;

            dim._current_x_pos = static_cast<int32>(a_x_pos);
            dim._current_y_pos = static_cast<int32>(a_y_pos);

            WindowMoveEvent event(old_x_pos, old_y_pos, dim._current_x_pos, dim._current_y_pos);
            user_data->_application_event_callback_func(event);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_window_resize_callback(GLFWwindow* a_window, int a_width, int a_height)
        {
            WindowInstance* user_data   = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            WindowDimension& dim        = user_data->_window_dimension;
            
            const int32 old_width     = dim._current_width;
            const int32 old_height    = dim._current_height;

            dim._current_width  = static_cast<int32>(a_width);
            dim._current_height = static_cast<int32>(a_height);

            glfwGetWindowFrameSize(a_window, &dim._window_frame_left, &dim._window_frame_top, &dim._window_frame_right, &dim._window_frame_bottom);

            WindowResizeEvent event(old_width, old_height, dim._current_width, dim._current_height);
            user_data->_application_event_callback_func(event);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_framebuffer_resize_callback(GLFWwindow* a_window, int a_width, int a_height)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            WindowDimension& dim = user_data->_window_dimension;

            const int32 old_frame_width   = dim._current_frame_width;
            const int32 old_frame_height  = dim._current_frame_height;

            dim._current_frame_width    = static_cast<int32>(a_width);
            dim._current_frame_height   = static_cast<int32>(a_height);

            bgfx::reset(a_width, a_height);
            bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

            WindowFramebufferResizeEvent event(old_frame_width, old_frame_height, dim._current_frame_width, dim._current_frame_height);
            user_data->_application_event_callback_func(event);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_set_clipboard_string(void* a_user_data, const char* a_text)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(a_user_data);
            glfwSetClipboardString(reinterpret_cast<GLFWwindow*>(a_user_data), a_text);

            InputClipboardEvent event(a_text);
            user_data->_application_event_callback_func(event);
        }

        const char* WindowManagementSystem::GLFWWindowCallBacks::glfw_get_clipboard_string(void* a_user_data)
        {
            return glfwGetClipboardString(reinterpret_cast<GLFWwindow*>(a_user_data));
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_set_item_drop_callback(GLFWwindow* a_window, int a_count, const char** a_paths)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            InputItemDropEvent event(a_count, a_paths);
            user_data->_application_event_callback_func(event);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_key_callback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            int32 key       = static_cast<int32>(a_key);
            int32 scancode  = static_cast<int32>(a_scancode);
            int32 action    = static_cast<int32>(a_action);
            int32 modifier  = static_cast<int32>(a_mods);

            ApplicationInstance::ProvideInputManagment()->SendKeyEvent(user_data->_id, key, scancode, action, modifier);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_char_callback(GLFWwindow* a_window, unsigned int a_char)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            uint16 character = static_cast<uint16>(a_char);

            ApplicationInstance::ProvideInputManagment()->SendCharEvent(user_data->_id, character);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_mousebutton_callback(GLFWwindow* a_window, int a_key, int a_action, int a_mods)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            int32 key       = static_cast<int32>(a_key);
            int32 action    = static_cast<int32>(a_action);
            int32 modifier  = static_cast<int32>(a_mods);
            int32 undefined = -1;

            ApplicationInstance::ProvideInputManagment()->SendMouseEvent(user_data->_id, key, undefined, action, modifier);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_mouse_callback(GLFWwindow* a_window, double a_x_pos, double a_y_pos)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            float32 x_pos = static_cast<float32>(a_x_pos);
            float32 y_pos = static_cast<float32>(a_y_pos);

            ApplicationInstance::ProvideInputManagment()->SendDirectionalEvent(user_data->_id, x_pos, y_pos);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_scroll_callback(GLFWwindow* a_window, double a_x_offset, double a_y_offset)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            float32 x_offset = static_cast<float32>(a_x_offset);
            float32 y_offset = static_cast<float32>(a_y_offset);

            ApplicationInstance::ProvideInputManagment()->SendScrollEvent(user_data->_id, x_offset, y_offset);
        }

#pragma endregion

    WindowManagementSystem::WindowManagementSystem(const std::string a_main_window_name)
        : _default_width(1280)
        , _default_height(720)
        , _default_window_name(a_main_window_name)
    {
    }

    WindowManagementSystem::~WindowManagementSystem()
    {
    }

    void WindowManagementSystem::InitWindowManagement()
    {        
        glfwSetErrorCallback(GLFWWindowCallBacks::glfw_error_callback);

        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        bgfx::renderFrame();
        
        WindowInstance* default_first_window = ConstructWindow(_default_width, _default_height, _default_window_name);
        glfwSetWindowUserPointer(default_first_window->_window, default_first_window);
        _default_first_window_id = default_first_window->_id;

        bgfx::PlatformData platform_data;
        platform_data.nwh = glfwGetWin32Window(default_first_window->_window);
        bgfx::setPlatformData(platform_data);
        
        bgfx::Init bgfx_init;
        bgfx_init.type              = bgfx::RendererType::Count; // Auto Choose Renderer
        bgfx_init.resolution.width  = _default_width;
        bgfx_init.resolution.height = _default_height;
        bgfx_init.resolution.reset  = BGFX_RESET_VSYNC;
        bgfx::init(bgfx_init);

        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        const bgfx::ViewId kClearView = 0;
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00cc6600);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    void WindowManagementSystem::TerminateWindowManagement()
    {
        bgfx::shutdown();
        glfwTerminate();
    }

    void WindowManagementSystem::BindApplicationEventFunc(const ApplicationEventCallbackFunc& a_event_callback_func)
    {
        _application_event_callback_func = a_event_callback_func;
    }

    bool WindowManagementSystem::HaveAllWindowsBeenClosed() const
    {
        return _window_instances.size() <= 0;
    }

    const DUID WindowManagementSystem::GetMainWindow() const
    {
        return _default_first_window_id;
    }

    WindowInstance* WindowManagementSystem::ConstructWindow(int32 a_width, int32 a_height, std::string  a_name)
    {
        WindowInstance* new_window = ConstructWindow(a_width, a_height, a_name.c_str());
        return new_window;
    }

    WindowInstance* WindowManagementSystem::ConstructWindow(int32 a_width, int32 a_height, const char* a_name)
    {
        WindowInstance new_window;

        GLFWwindow* glfw_window                     = glfwCreateWindow(a_width, a_height, a_name, NULL, NULL);
        new_window._id                              = DFW::GenerateDUID();
        new_window._window                          = glfw_window;
        new_window._application_event_callback_func = _application_event_callback_func;
        new_window._name                            = a_name;
        new_window._is_focussed                     = true;

        SetFocussedWindowID(new_window._id);

        glfwSetWindowFocusCallback(glfw_window, GLFWWindowCallBacks::glfw_window_focus_callback);
        glfwSetWindowIconifyCallback(glfw_window, GLFWWindowCallBacks::glfw_window_minimized_callback);

        glfwSetWindowCloseCallback(glfw_window, GLFWWindowCallBacks::glfw_window_closed_callback);

        glfwSetWindowPosCallback(glfw_window, GLFWWindowCallBacks::glfw_window_position_callback);
        glfwSetWindowSizeCallback(glfw_window, GLFWWindowCallBacks::glfw_window_resize_callback);
        glfwSetFramebufferSizeCallback(glfw_window, GLFWWindowCallBacks::glfw_framebuffer_resize_callback);

        glfwSetDropCallback(glfw_window, GLFWWindowCallBacks::glfw_set_item_drop_callback);

        glfwSetKeyCallback(glfw_window, GLFWWindowCallBacks::glfw_key_callback);
        glfwSetCharCallback(glfw_window, GLFWWindowCallBacks::glfw_char_callback);
        glfwSetMouseButtonCallback(glfw_window, GLFWWindowCallBacks::glfw_mousebutton_callback);
        glfwSetCursorPosCallback(glfw_window, GLFWWindowCallBacks::glfw_mouse_callback);
        glfwSetScrollCallback(glfw_window, GLFWWindowCallBacks::glfw_scroll_callback);
        
        ApplicationInstance::ProvideInputManagment()->RegisterWindow(&new_window);

        auto [it_pair, result] =_window_instances.emplace(new_window._id, new_window);
        if (result)
        {
            return &(it_pair->second);
        }
        else
        {
            DFW_ERRORLOG("Window Management System couldn't construct a new window.");
            return nullptr;
        }
    }

    void WindowManagementSystem::ChangeDefaultWindowName(const std::string a_new_default_window_name)
    {
        ChangeDefaultWindowName(a_new_default_window_name.c_str());
    }

    void WindowManagementSystem::ChangeDefaultWindowName(const char* a_new_default_window_name)
    {
        _default_window_name = a_new_default_window_name;
    }

    const WindowInstance* WindowManagementSystem::CurrentFocussedWindow() const
    {
        auto it = _window_instances.find(_current_focussed_window_id);
        if (it != _window_instances.end())
        {
            return &(*it).second;
        }
        else
        {
            return nullptr;
        }
    }

    bool WindowManagementSystem::IsWindowFocussed(WindowID a_window_id) const
    {
        return a_window_id == _current_focussed_window_id;
    }

    bool WindowManagementSystem::IsWindowMinimized(const WindowID a_window_id) const
    {
        auto it = _window_instances.find(a_window_id);
        if (it != _window_instances.end())
        {
            return (*it).second._is_minimized;
        }
        else
        {
            return false;
        }
    }

    void WindowManagementSystem::DestructWindow(WindowInstance* a_window)
    {
        if (a_window->_id == _current_focussed_window_id)
            SetFocussedWindowID(WindowID());

        glfwDestroyWindow(a_window->_window);

        ApplicationInstance::ProvideInputManagment()->UnregisterWindow(a_window);

        _window_instances.erase(a_window->_id);
    }

    void WindowManagementSystem::SetFocussedWindowID(WindowID a_window_id)
    {
        bool is_first       = _current_focussed_window_id.is_nil();
        bool is_not_same    = a_window_id != _current_focussed_window_id;
        if (!is_first && is_not_same)
        {
            auto it = _window_instances.find(_current_focussed_window_id);
            if (it != _window_instances.end())
            {
                (*it).second._is_focussed = false;
            }
        }

        _current_focussed_window_id = a_window_id;
    }

} // End of namespace ~ DFW.
