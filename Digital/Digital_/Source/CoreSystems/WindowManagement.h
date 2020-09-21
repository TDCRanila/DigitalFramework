#pragma once

#include <Defines/IDDefines.h>

#include <string>
#include <unordered_map>

struct GLFWwindow;

namespace DCore
{
    // Forward Declare(s)
    struct InputUserData;

    namespace GLFWWindowCallBacks 
    {
        static void glfw_error_callback(int error, const char* description);
        
        static void glfw_window_closed_callback(GLFWwindow* a_window);

        static void glfw_window_focus_callback(GLFWwindow* a_window, int a_focused);
        static void glfw_window_iconify_callback(GLFWwindow* a_window, int a_iconified);

        static void glfw_window_position_callback(GLFWwindow* a_window, int a_x_pos, int a_y_pos);
        static void glfw_window_resize_callback(GLFWwindow* a_window, int a_width, int a_height);

        static void glfw_framebuffer_resize_callback();
        static void glfw_window_refresh_callback();

        static const char* glfw_set_clipboard_string(void* a_user_data, const char* a_text);
        static const char* glfw_get_clipboard_string(void* a_user_data);

        static void glfw_key_callback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods);
        static void glfw_char_callback(GLFWwindow* a_window, unsigned int a_char);
        static void glfw_mouse_callback(GLFWwindow* a_window, double a_x_pos, double a_y_pos);
        static void glfw_mousebutton_callback(GLFWwindow* a_window, int a_key, int a_action, int a_mods);
        static void glfw_scroll_callback(GLFWwindow* a_window, double a_x_offset, double a_y_offset);
    } // End of namespace ~ GLFWWindowCallBacks

    struct WindowDimension
    {
        WindowDimension();

        int32 _current_frame_width, _current_frame_height;
        int32 _current_width, _current_height;
        int32 _current_x_pos, _current_y_pos; // Point is the the upper-left corner.
    };

    struct WindowInstance
    {
        WindowInstance();

        WindowDimension _window_dimension;

        WindowID        _id;
        std::string     _name;

        GLFWwindow*     _window;
        
        bool _should_be_closed;
        bool _is_iconified;
        bool _is_focussed;
    };

    class WindowManagementSystem
    {
    public:
        ~WindowManagementSystem();

        void InitWindowManagement();
        void TerminateWindowManagement();

        const DUID GetMainWindow() const;

        WindowInstance* ConstructWindow(const int32 a_width, const int32 a_height, const std::string  a_name);
        WindowInstance* ConstructWindow(const int32 a_width, const int32 a_height, const char*  a_name);
        
        void DestructWindow(const std::string  a_name);
        void DestructWindow(const char* a_name);

        void ChangeDefaultWindowName(const char* a_new_default_window_name);
        void ChangeDefaultWindowName(const std::string a_new_default_window_name);

        void ChangeWindowName(WindowID a_window_id, const std::string a_new_window_name);
    
        std::unordered_map<WindowID, WindowInstance> _window_instances;
    protected:
        friend class ApplicationInstance;

        WindowManagementSystem(const std::string a_window_name = "DIGITAL");

    private:

        std::string _default_window_name;

        DUID _default_first_window_id;

        int32 _default_width;
        int32 _default_height;

    };

} // End of namespace ~ DCore
