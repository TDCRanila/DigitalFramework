#include <CoreSystems/Window/Windows/WindowManagementGLFW.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Logging/Logger.h>
#include <Defines/Defines.h>

#include <CoreSystems/Memory.h>

namespace DFW
{
    namespace DWindow
    {
        namespace GLFWWindowCallBacks
        {
            static EventDispatcher* application_event_dispatcher        = nullptr;
            static DFW::DInput::InputManagementSystem* input_system     = nullptr;
            static DFW::DWindow::WindowManagementGLFW* window_system    = nullptr;

            static void glfw_error_callback(int error, const char* description)
            {
                UNUSED(error);
                fprintf(stderr, "GLFW Error: %s\n", description);
            }

            static void glfw_window_closed_callback(GLFWwindow* a_window)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
                user_data->_should_be_closed = true;

                window_system->RequestWindowClose(user_data->_id);

                application_event_dispatcher->InstantBroadcast<WindowFocusEvent>(user_data->_is_focussed);
            }

            static void glfw_window_focus_callback(GLFWwindow* a_window, int a_result)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
                user_data->_is_focussed = static_cast<bool>(a_result);

                application_event_dispatcher->InstantBroadcast<WindowFocusEvent>(user_data->_is_focussed);
            }

            static void glfw_window_minimized_callback(GLFWwindow* a_window, int a_result)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
                user_data->_is_minimized = static_cast<bool>(a_result);

                application_event_dispatcher->InstantBroadcast<WindowMinimizedEvent>(user_data->_is_focussed);
            }

            static void glfw_window_position_callback(GLFWwindow* a_window, int a_x_pos, int a_y_pos)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
                WindowDimension& dim = user_data->_window_dimension;

                const int32 old_x_pos = dim._current_x_pos;
                const int32 old_y_pos = dim._current_y_pos;

                dim._current_x_pos = static_cast<int32>(a_x_pos);
                dim._current_y_pos = static_cast<int32>(a_y_pos);

                application_event_dispatcher->InstantBroadcast<WindowMoveEvent>(
                        old_x_pos
                    ,   old_y_pos
                    ,   dim._current_x_pos
                    ,   dim._current_y_pos
                    );
            }

            static void glfw_window_resize_callback(GLFWwindow* a_window, int a_width, int a_height)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
                WindowDimension& dim = user_data->_window_dimension;

                const int32 old_width = dim._current_width;
                const int32 old_height = dim._current_height;

                dim._current_width = static_cast<int32>(a_width);
                dim._current_height = static_cast<int32>(a_height);

                glfwGetWindowFrameSize(a_window, &dim._window_frame_left, &dim._window_frame_top, &dim._window_frame_right, &dim._window_frame_bottom);

                application_event_dispatcher->InstantBroadcast<WindowResizeEvent>(
                        old_width
                    ,   old_height
                    ,   dim._current_width
                    ,   dim._current_height
                    );
            }

            static void glfw_framebuffer_resize_callback(GLFWwindow* a_window, int a_width, int a_height)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
                WindowDimension& dim = user_data->_window_dimension;

                const int32 old_frame_width = dim._current_frame_width;
                const int32 old_frame_height = dim._current_frame_height;

                dim._current_frame_width = static_cast<int32>(a_width);
                dim._current_frame_height = static_cast<int32>(a_height);

                application_event_dispatcher->InstantBroadcast<WindowFramebufferResizeEvent>(
                        old_frame_width
                    ,   old_frame_height
                    ,   dim._current_frame_width
                    ,   dim._current_frame_height
                    );
            }

            static void glfw_set_clipboard_string(void* a_user_data, const char* a_text)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(a_user_data);
                glfwSetClipboardString(reinterpret_cast<GLFWwindow*>(a_user_data), a_text);

                application_event_dispatcher->InstantBroadcast<InputClipboardEvent>(a_text);
            }

            static const char* glfw_get_clipboard_string(void* a_user_data)
            {
                return glfwGetClipboardString(reinterpret_cast<GLFWwindow*>(a_user_data));
            }

            static void glfw_set_item_drop_callback(GLFWwindow* a_window, int a_count, const char** a_paths)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

                application_event_dispatcher->InstantBroadcast<InputItemDropEvent>(a_count, a_paths);
            }

            static void glfw_key_callback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

                int32 key       = static_cast<int32>(a_key);
                int32 scancode  = static_cast<int32>(a_scancode);
                int32 action    = static_cast<int32>(a_action);
                int32 modifier  = static_cast<int32>(a_mods);

                input_system->SendKeyEvent(user_data->_id, key, scancode, action, modifier);
            }

            static void glfw_char_callback(GLFWwindow* a_window, unsigned int a_char)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

                uint16 character = static_cast<uint16>(a_char);

                input_system->SendCharEvent(user_data->_id, character);
            }

            static void glfw_mousebutton_callback(GLFWwindow* a_window, int a_key, int a_action, int a_mods)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

                int32 key       = static_cast<int32>(a_key);
                int32 action    = static_cast<int32>(a_action);
                int32 modifier  = static_cast<int32>(a_mods);
                int32 undefined = -1;

                input_system->SendMouseEvent(user_data->_id, key, undefined, action, modifier);
            }

            static void glfw_mouse_callback(GLFWwindow* a_window, double a_x_pos, double a_y_pos)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

                float32 x_pos = static_cast<float32>(a_x_pos);
                float32 y_pos = static_cast<float32>(a_y_pos);

                input_system->SendDirectionalEvent(user_data->_id, x_pos, y_pos);
            }

            static void glfw_scroll_callback(GLFWwindow* a_window, double a_x_offset, double a_y_offset)
            {
                WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

                float32 x_offset = static_cast<float32>(a_x_offset);
                float32 y_offset = static_cast<float32>(a_y_offset);

                input_system->SendScrollEvent(user_data->_id, x_offset, y_offset);
            }

        } // End of namespace ~ GLFWWindowCallBacks.

        void WindowManagementGLFW::InitWindowManagement()
        {
            glfwSetErrorCallback(GLFWWindowCallBacks::glfw_error_callback);

            glfwInit();

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            // Saving Core Services.
            GLFWWindowCallBacks::application_event_dispatcher   = CoreService::GetMainEventHandler();
            GLFWWindowCallBacks::input_system                   = CoreService::GetInputSystem();
            GLFWWindowCallBacks::window_system                  = static_cast<WindowManagementGLFW*>(CoreService::GetWindowSystem());
            
            // Construct the main window.
            SharedPtr<WindowInstance> const new_window = ConstructWindow(default_window_parameters);

            _main_window_id = new_window->_id;
        }

        void WindowManagementGLFW::TerminateWindowManagement()
        {
            glfwTerminate();
        }

        void WindowManagementGLFW::PollWindowEvents()
        {
            glfwPollEvents();
        }

        SharedPtr<WindowInstance> WindowManagementGLFW::ConstructWindow(WindowParameters const& a_window_parameters)
        {
            const char* name = a_window_parameters.name.c_str();
            GLFWwindow* glfw_window = glfwCreateWindow(a_window_parameters.width, a_window_parameters.height, name, NULL, NULL);
            DFW_ASSERT(!glfw_window, "Unable to create a new glfw window.");

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

            SharedPtr<WindowInstanceGLFW> new_window = MakeShared<WindowInstanceGLFW>();
            new_window->_id             = DFW::GenerateDUID();
            new_window->_glfw_window    = glfw_window;
            new_window->_name           = a_window_parameters.name;

            glfwSetWindowUserPointer(new_window->_glfw_window, new_window.get());

            // TODO Replace direct call to event-based using a 'window created' event.
            CoreService::GetInputSystem()->RegisterWindow(new_window.get());

            _window_instances.emplace(new_window->_id, new_window);
            return new_window;
        }

        void WindowManagementGLFW::RequestWindowClose(WindowID const a_window_id)
        {
            SharedPtr<WindowInstanceGLFW> const& window_ptr = std::static_pointer_cast<WindowInstanceGLFW>(GetWindowInternal(a_window_id));
            
            glfwDestroyWindow(window_ptr->_glfw_window);
            _window_instances.erase(window_ptr->_id);

            // TODO Replace direct call to event-based using a 'window created' event.
            CoreService::GetInputSystem()->UnregisterWindow(window_ptr->_id);
        }

        void* WindowManagementGLFW::GetWindowNWH(WindowID const a_window_id) const
        {
            return std::static_pointer_cast<WindowInstanceGLFW>(GetWindow(a_window_id))->_glfw_window;
        }

        void* WindowManagementGLFW::GetWindowPWH(WindowID const a_window_id) const
        {
            return glfwGetWin32Window(std::static_pointer_cast<WindowInstanceGLFW>(GetWindow(a_window_id))->_glfw_window);
        }

        void WindowManagementGLFW::ChangeWindowParameters(WindowID const a_window_id, WindowParameters const& a_window_parameters)
        {
            SharedPtr<WindowInstanceGLFW> const& window_ptr = std::static_pointer_cast<WindowInstanceGLFW>(GetWindowInternal(a_window_id));

            // Name
            window_ptr->_name = a_window_parameters.name;

            // Window Resize
            // Lend the glfw window callback
            GLFWwindow* const glfw_window = window_ptr.get()->_glfw_window;
            GLFWWindowCallBacks::glfw_window_resize_callback(glfw_window, a_window_parameters.width, a_window_parameters.height);
        }

    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
