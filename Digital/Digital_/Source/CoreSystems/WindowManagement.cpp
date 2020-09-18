#include <CoreSystems/WindowManagement.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <Defines/Defines.h>
#include <CoreSystems/InputManagement.h>

namespace DCore
{

    namespace GLFWWindowCallBacks
    {
        void glfw_error_callback(int error, const char* description)
        {
            fprintf(stderr, "GLFW Error: %s\n", description);
        }

        static void glfw_window_closed_callback(GLFWwindow* a_window)
        {

        }

        static void glfw_window_focus_callback(GLFWwindow* a_window, int a_result)
        {

        }

        static void glfw_window_iconify_callback(GLFWwindow* a_window, int a_result)
        {

        }

        static void glfw_window_closed_callback(GLFWwindow* a_window, int a_result)
        {

        }

        static void glfw_window_position_callback(GLFWwindow* a_window, int a_x_pos, int a_y_pos)
        {

        }

        static void glfw_window_resize_callback(GLFWwindow* a_window, int a_width, int a_height)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            WindowDimension& dim = user_data->_window_dimensions;
            
            dim._current_width = a_width;
            dim._current_height = a_height;
            
            int framebuffer_width, framebuffer_height;
            glfwGetFramebufferSize(user_data->_window, &framebuffer_width, &framebuffer_height);
            dim._current_frame_width = framebuffer_width;
            dim._current_frame_height = framebuffer_height;
            
            INFOLOG("H: " << a_height << " W: " << a_width);
            INFOLOG("fH: " << framebuffer_height << " fW: " << framebuffer_width);
        }

        static const char* glfw_set_clipboard_string(void* a_user_data, const char* a_text)
        {
            glfwSetClipboardString(reinterpret_cast<GLFWwindow*>(a_user_data), a_text);
        }

        static const char* glfw_get_clipboard_string(void* a_user_data)
        {
            glfwGetClipboardString(reinterpret_cast<GLFWwindow*>(a_user_data));
        }

        void glfw_key_callback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods) 
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_input_data->_input_system_ptr->SendKeyboardEvent(user_data->_input_data, a_key, a_scancode, a_action, a_mods);
        }

        void glfw_char_callback(GLFWwindow* a_window, unsigned int a_char)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_input_data->_input_system_ptr->SendCharEvent(user_data->_input_data, a_char);
        }

        void glfw_mousebutton_callback(GLFWwindow* a_window, int a_key, int a_action, int a_mods) 
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_input_data->_input_system_ptr->SendMouseEvent(user_data->_input_data, a_key, a_action, a_mods);
        }

        void glfw_mouse_callback(GLFWwindow* a_window, double a_x_pos, double a_y_pos) 
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_input_data->_input_system_ptr->SendCursorEvent(user_data->_input_data, a_x_pos, a_y_pos);
        }

        void glfw_scroll_callback(GLFWwindow* a_window, double a_x_offset, double a_y_offset) 
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            user_data->_input_data->_input_system_ptr->SendScrollEvent(user_data->_input_data, a_x_offset, a_y_offset);
        }

    } // End of namespace ~ GLFWWindowCallBacks

    WindowDimension::WindowDimension() :
        _current_height(1280),
        _current_width(720),
        _current_frame_height(1280),
        _current_frame_width(720),
        _current_x_pos(0),
        _current_y_pos(0)
    {
    }

    WindowInstance::WindowInstance()  :
        _window(nullptr),
        _input_data(nullptr),
        _name(""),
        _should_be_closed(false),
        _iconified(false),
        _focussed(false)
    {
        GLFWwindow* _window;
        InputUserData* _input_data;
        std::string _name;

        bool _should_be_closed;
        bool _iconified;
        bool _focussed;
        int32 _current_width, _current_height;
        int32 _current_x_pos, _current_y_pos; // Point is the the upper-left corner.
    }

    WindowManagementSystem::WindowManagementSystem(const std::string a_main_window_name)
        :   _input_management(nullptr),
            _default_width(1280),
            _default_height(720),
            _default_window_name(a_main_window_name)
    {
    }

    WindowManagementSystem::WindowManagementSystem(const char* a_main_window_name)
        :   _input_management(nullptr),
            _default_width(1280),
            _default_height(720),
            _default_window_name(a_main_window_name)
    {
    }

    WindowManagementSystem::~WindowManagementSystem()
    {

    }

    void WindowManagementSystem::ProvideInputSystem(InputManagementSystem* a_input_system_ptr)
    {
        if (!a_input_system_ptr)
        {
            ERRORLOG("Window Management System has been provided an invalid input system.");
        }

        if (_input_management)
        {
            WARNLOG("Window Management System already has a valid input system provided.");
        }
        else
        {
            _input_management = a_input_system_ptr;
        }
    }

    void WindowManagementSystem::InitWindowManagement()
    {
        if (!_input_management)
        {
            ERRORLOG("Window Management System has no valid input system provided.");
            return;
        }

        // Setting callbacks
        glfwSetErrorCallback(GLFWWindowCallBacks::glfw_error_callback);

        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        bgfx::renderFrame();
        
        WindowInstance* new_window  = ConstructWindow(_default_width, _default_height, _default_window_name);
        InputUserData* input_data   = _input_management->ProvideInputUserData(new_window);
        new_window->_input_data = input_data;
        glfwSetWindowUserPointer(new_window->_window, new_window);

        bgfx::PlatformData platform_data;
        platform_data.nwh = glfwGetWin32Window(_window_instances[0]._window);
        bgfx::setPlatformData(platform_data);
        
        bgfx::Init bgfx_init;
        bgfx_init.type              = bgfx::RendererType::Count; // Auto Choose Renderer
        bgfx_init.resolution.width  = _default_width;
        bgfx_init.resolution.height = _default_height;
        bgfx_init.resolution.reset  = BGFX_RESET_VSYNC;
        bgfx::init(bgfx_init);

        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        const bgfx::ViewId kClearView = 0;
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR, 0x00cc6600);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    void WindowManagementSystem::TerminateWindowManagement()
    {
        bgfx::shutdown();
        glfwTerminate();
    }

    WindowInstance* WindowManagementSystem::ConstructWindow(int32 a_width, int32 a_height, std::string  a_name)
    {
        WindowInstance* new_window = ConstructWindow(a_width, a_height, a_name.c_str());
        return new_window;
    }

    WindowInstance* WindowManagementSystem::ConstructWindow(int32 a_width, int32 a_height, const char* a_name)
    {
        WindowInstance new_window;
        GLFWwindow* glfw_window = glfwCreateWindow(a_width, a_height, a_name, NULL, NULL);
        new_window._window  = glfw_window;
        new_window._name    = a_name;

        glfwSetWindowFocusCallback(glfw_window, GLFWWindowCallBacks::glfw_window_focus_callback);
        glfwSetWindowIconifyCallback(glfw_window, GLFWWindowCallBacks::glfw_window_iconify_callback);

        glfwSetWindowCloseCallback(glfw_window, GLFWWindowCallBacks::glfw_window_closed_callback);

        glfwSetWindowPosCallback(glfw_window, GLFWWindowCallBacks::glfw_window_position_callback);
        glfwSetWindowSizeCallback(glfw_window, GLFWWindowCallBacks::glfw_window_resize_callback);

        glfwSetKeyCallback(glfw_window, GLFWWindowCallBacks::glfw_key_callback);
        glfwSetCharCallback(glfw_window, GLFWWindowCallBacks::glfw_char_callback);
        glfwSetMouseButtonCallback(glfw_window, GLFWWindowCallBacks::glfw_mousebutton_callback);
        glfwSetCursorPosCallback(glfw_window, GLFWWindowCallBacks::glfw_mouse_callback);
        glfwSetScrollCallback(glfw_window, GLFWWindowCallBacks::glfw_scroll_callback);

        _window_instances[0] = new_window;
        return &_window_instances[0];
    }

    void WindowManagementSystem::DestructWindow(std::string  a_name)
    {

    }

    void WindowManagementSystem::DestructWindow(const char* a_name)
    {
        
    }

    void WindowManagementSystem::ChangeDefaultWindowName(const std::string a_new_default_window_name)
    {
        ChangeDefaultWindowName(a_new_default_window_name.c_str());
    }

    void WindowManagementSystem::ChangeDefaultWindowName(const char* a_new_default_window_name)
    {
        _default_window_name = a_new_default_window_name;
    }

} // End of namespace ~ DCore