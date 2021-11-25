#pragma once

#include <CoreSystems/Window/WindowData.h>

#include <CoreSystems/Memory.h>

// Forward Declare(s)
struct GLFWwindow;

namespace DFW
{
    // FW Declare.
    class ApplicationInstance;

    namespace DWindow
    {
        constexpr const char*   DFW_DEFAULT_WINDOW_NAME     = "DIGITAL";
        constexpr int32         DFW_DEFAULT_WINDOW_WIDTH    = 1280;
        constexpr int32         DFW_DEFAULT_WINDOW_HEIGHT   = 720;

        using WindowContainer = std::unordered_map<WindowID, SharedPtr<WindowInstance>>;

        class WindowManagementSystem
        {
        public:
            class GLFWWindowCallBacks
            {
            public:
                static void glfw_error_callback(int error, const char* description);

                static void glfw_window_closed_callback(GLFWwindow* a_window);
                static void glfw_window_focus_callback(GLFWwindow* a_window, int a_focused);
                static void glfw_window_minimized_callback(GLFWwindow* a_window, int a_iconified);

                static void glfw_window_position_callback(GLFWwindow* a_window, int a_x_pos, int a_y_pos);
                static void glfw_window_resize_callback(GLFWwindow* a_window, int a_width, int a_height);
                static void glfw_framebuffer_resize_callback(GLFWwindow* a_window, int a_width, int a_height);

                static void glfw_framebuffer_resize_callback();
                static void glfw_window_refresh_callback();

                static void glfw_set_clipboard_string(void* a_user_data, const char* a_text);
                static const char* glfw_get_clipboard_string(void* a_user_data);

                static void glfw_set_item_drop_callback(GLFWwindow* a_window, int a_count, const char** a_paths);

                static void glfw_key_callback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods);
                static void glfw_char_callback(GLFWwindow* a_window, unsigned int a_char);
                static void glfw_mouse_callback(GLFWwindow* a_window, double a_x_pos, double a_y_pos);
                static void glfw_mousebutton_callback(GLFWwindow* a_window, int a_key, int a_action, int a_mods);
                static void glfw_scroll_callback(GLFWwindow* a_window, double a_x_offset, double a_y_offset);
            private:
                GLFWWindowCallBacks() = delete;
            }; // End of "namespace" ~ GLFWWindowCallBacks

        public:
            ~WindowManagementSystem() = default;

            SharedPtr<WindowInstance> ConstructWindow(WindowParameters const& a_window_parameters);

            WindowID GetMainWindowID() const;
            SharedPtr<WindowInstance> const GetMainWindow() const;
            SharedPtr<WindowInstance> const GetWindow(WindowID const a_window_id) const;
            void* GetMainWindowNWH() const;

            bool HaveAllWindowsBeenClosed() const;
            void RequestWindowClose(std::string const& a_name);
            void RequestWindowClose(WindowID const a_window_id);

            WindowID CurrentFocussedWindowID() const;
            bool IsWindowFocussed(WindowID const a_window_id) const;
            bool IsWindowMinimized(WindowID const a_window_id) const;

            void SetDefaultWindowParameters(WindowParameters const& a_window_parameters);
            void ChangeWindowParameters(WindowID const a_window_id, WindowParameters const& a_window_parameters);

        private:
            friend ApplicationInstance;

            WindowManagementSystem() = default;

            void InitWindowManagement();
            void TerminateWindowManagement();
            void PollWindowEvents();

            void BindApplicationEventFunc(ApplicationEventCallbackFunc const& a_event_callback_func);

        private:
            SharedPtr<WindowInstance>& GetWindowInternal(WindowID const a_window_id);
            void DestructWindow(WindowInstance* const a_window_ptr);
            void SetFocussedWindowID(WindowID const a_window_id);

            WindowContainer _window_instances;
            ApplicationEventCallbackFunc _application_event_callback_func;

            WindowParameters _default_window_paramters;
            WindowID _current_focussed_window_id;
            WindowID _main_window_id;
        };

    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
