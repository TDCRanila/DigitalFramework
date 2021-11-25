#include <CoreSystems/Window/WindowManagement.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Logging/Logger.h>
#include <Defines/Defines.h>

namespace DFW
{
    namespace DWindow
    {
#pragma region GLFWWindowCallBacks
        void WindowManagementSystem::GLFWWindowCallBacks::glfw_error_callback(int error, const char* description)
        {
            UNUSED(error);
            fprintf(stderr, "GLFW Error: %s\n", description);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_window_closed_callback(GLFWwindow* a_window)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_should_be_closed = true;

            WindowCloseEvent event;
            user_data->_application_event_callback_func(event);

            CoreService::GetWindowSystem()->DestructWindow(user_data);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_window_focus_callback(GLFWwindow* a_window, int a_result)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_is_focussed = static_cast<bool>(a_result);

            WindowFocusEvent event(user_data->_is_focussed);
            user_data->_application_event_callback_func(event);

            if (user_data->_is_focussed)
                CoreService::GetWindowSystem()->SetFocussedWindowID(user_data->_id);
            else
                CoreService::GetWindowSystem()->SetFocussedWindowID(WindowID());
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_window_minimized_callback(GLFWwindow* a_window, int a_result)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_is_minimized = static_cast<bool>(a_result);

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
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            WindowDimension& dim = user_data->_window_dimension;

            const int32 old_width = dim._current_width;
            const int32 old_height = dim._current_height;

            dim._current_width = static_cast<int32>(a_width);
            dim._current_height = static_cast<int32>(a_height);

            glfwGetWindowFrameSize(a_window, &dim._window_frame_left, &dim._window_frame_top, &dim._window_frame_right, &dim._window_frame_bottom);

            WindowResizeEvent event(old_width, old_height, dim._current_width, dim._current_height);
            user_data->_application_event_callback_func(event);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_framebuffer_resize_callback(GLFWwindow* a_window, int a_width, int a_height)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            WindowDimension& dim = user_data->_window_dimension;

            const int32 old_frame_width = dim._current_frame_width;
            const int32 old_frame_height = dim._current_frame_height;

            dim._current_frame_width = static_cast<int32>(a_width);
            dim._current_frame_height = static_cast<int32>(a_height);

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

            int32 key = static_cast<int32>(a_key);
            int32 scancode = static_cast<int32>(a_scancode);
            int32 action = static_cast<int32>(a_action);
            int32 modifier = static_cast<int32>(a_mods);

            CoreService::GetInputSystem()->SendKeyEvent(user_data->_id, key, scancode, action, modifier);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_char_callback(GLFWwindow* a_window, unsigned int a_char)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            uint16 character = static_cast<uint16>(a_char);

            CoreService::GetInputSystem()->SendCharEvent(user_data->_id, character);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_mousebutton_callback(GLFWwindow* a_window, int a_key, int a_action, int a_mods)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            int32 key = static_cast<int32>(a_key);
            int32 action = static_cast<int32>(a_action);
            int32 modifier = static_cast<int32>(a_mods);
            int32 undefined = -1;

            CoreService::GetInputSystem()->SendMouseEvent(user_data->_id, key, undefined, action, modifier);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_mouse_callback(GLFWwindow* a_window, double a_x_pos, double a_y_pos)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            float32 x_pos = static_cast<float32>(a_x_pos);
            float32 y_pos = static_cast<float32>(a_y_pos);

            CoreService::GetInputSystem()->SendDirectionalEvent(user_data->_id, x_pos, y_pos);
        }

        void WindowManagementSystem::GLFWWindowCallBacks::glfw_scroll_callback(GLFWwindow* a_window, double a_x_offset, double a_y_offset)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            float32 x_offset = static_cast<float32>(a_x_offset);
            float32 y_offset = static_cast<float32>(a_y_offset);

            CoreService::GetInputSystem()->SendScrollEvent(user_data->_id, x_offset, y_offset);
        }

#pragma endregion

        void WindowManagementSystem::InitWindowManagement()
        {
            glfwSetErrorCallback(GLFWWindowCallBacks::glfw_error_callback);

            glfwInit();

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            SharedPtr<WindowInstance> const new_window = ConstructWindow(
                WindowParameters(DFW_DEFAULT_WINDOW_NAME, DFW_DEFAULT_WINDOW_WIDTH, DFW_DEFAULT_WINDOW_HEIGHT)
            );

            _main_window_id = new_window->_id;
        }

        void WindowManagementSystem::TerminateWindowManagement()
        {
            glfwTerminate();
        }

        void WindowManagementSystem::PollWindowEvents()
        {
            glfwPollEvents();
        }

        void WindowManagementSystem::BindApplicationEventFunc(ApplicationEventCallbackFunc const& a_event_callback_func)
        {
            _application_event_callback_func = a_event_callback_func;
        }

        SharedPtr<WindowInstance> WindowManagementSystem::ConstructWindow(WindowParameters const& a_window_parameters)
        {

            const char* name = a_window_parameters.name.c_str();
            GLFWwindow* glfw_window = glfwCreateWindow(a_window_parameters.width, a_window_parameters.height, name, NULL, NULL);

            // Setting-Up GLFW Window Callbacks
            {
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
            }

            SharedPtr<WindowInstance> new_window = MakeShared<WindowInstance>();
            new_window->_id             = DFW::GenerateDUID();
            new_window->_window         = glfw_window;
            new_window->_name           = a_window_parameters.name;

            new_window->_application_event_callback_func = _application_event_callback_func;

            glfwSetWindowUserPointer(new_window->_window, new_window.get());

            CoreService::GetInputSystem()->RegisterWindow(new_window.get());
            
            SetFocussedWindowID(new_window->_id);

            _window_instances.emplace(new_window->_id, new_window);
            return new_window;
        }

        WindowID WindowManagementSystem::GetMainWindowID() const
        {
            return _main_window_id;
        }

        SharedPtr<WindowInstance> const WindowManagementSystem::GetMainWindow() const
        {
            return _window_instances.at(_main_window_id);
        }

        SharedPtr<WindowInstance> const WindowManagementSystem::GetWindow(WindowID const a_window_id) const
        {
            if (auto const it = _window_instances.find(a_window_id); it != _window_instances.end())
            {
                return (*it).second;
            }
            else
            {
                DFW_ERRORLOG("Attempting to find a window with ID: {}, but it cannot be found.", a_window_id);
                return SharedPtr<WindowInstance>();
            }
        }

        void* WindowManagementSystem::GetMainWindowNWH() const
        {
            return glfwGetWin32Window(GetWindow(_main_window_id)->_window);
        }

        bool WindowManagementSystem::HaveAllWindowsBeenClosed() const
        {
            return _window_instances.size() <= 0;
        }

        WindowID WindowManagementSystem::CurrentFocussedWindowID() const
        {
            return _current_focussed_window_id;
        }

        bool WindowManagementSystem::IsWindowFocussed(WindowID a_window_id) const
        {
            return a_window_id == _current_focussed_window_id;
        }

        bool WindowManagementSystem::IsWindowMinimized(WindowID const a_window_id) const
        {
            return GetWindow(a_window_id)->_is_minimized;
        }

        void WindowManagementSystem::SetDefaultWindowParameters(WindowParameters const& a_window_parameters)
        {
            _default_window_paramters = a_window_parameters;
        }

        void WindowManagementSystem::ChangeWindowParameters(WindowID const a_window_id, WindowParameters const& a_window_parameters)
        {
            SharedPtr<WindowInstance>& window_ptr = GetWindowInternal(a_window_id);

            // Name
            window_ptr->_name = a_window_parameters.name;
            
            // Window Resize
            // Lend the glfw window callback
            GLFWwindow* const glfw_window = window_ptr.get()->_window;
            GLFWWindowCallBacks::glfw_window_resize_callback(glfw_window, a_window_parameters.width, a_window_parameters.height);
        }

        SharedPtr<WindowInstance>& WindowManagementSystem::GetWindowInternal(WindowID const a_window_id)
        {
            if (auto const it = _window_instances.find(a_window_id); it != _window_instances.end())
            {
                return (*it).second;
            }
            else
            {
                DFW_ERRORLOG("Attempting to find a window with ID: {}, but it cannot be found.", a_window_id);
                DFW_ASSERT((*it).second);
                SharedPtr<WindowInstance> ptr;
                return ptr;
            }
        }

        void WindowManagementSystem::DestructWindow(WindowInstance* const a_window_ptr)
        {
            DFW_ASSERT(a_window_ptr);

            if (a_window_ptr->_id == _current_focussed_window_id)
                SetFocussedWindowID(WindowID());

            glfwDestroyWindow(a_window_ptr->_window);
            _window_instances.erase(a_window_ptr->_id);

            CoreService::GetInputSystem()->UnregisterWindow(a_window_ptr->_id);
        }

        void WindowManagementSystem::SetFocussedWindowID(WindowID const a_window_id)
        {
            bool is_first = _current_focussed_window_id.is_nil();

            bool is_not_same = a_window_id != _current_focussed_window_id;

            if (!is_first && is_not_same)
            {
                auto it = _window_instances.find(_current_focussed_window_id);
                if (it != _window_instances.end())
                {
                    (*it).second->_is_focussed = false;
                }
            }

            _current_focussed_window_id = a_window_id;
        }
    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
