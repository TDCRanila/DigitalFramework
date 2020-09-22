#include <CoreSystems/WindowManagement.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/InputManagement.h>
#include <Defines/Defines.h>

namespace DCore
{

    namespace GLFWWindowCallBacks
    {
        void glfw_error_callback(int error, const char* description)
        {
            UNUSED(error);
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
            WindowInstance* user_data   = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));
            WindowDimension& dim        = user_data->_window_dimension;
            
            dim._current_width  = a_width;
            dim._current_height = a_height;
            
            int framebuffer_width, framebuffer_height;
            glfwGetFramebufferSize(user_data->_window, &framebuffer_width, &framebuffer_height);
            dim._current_frame_width    = framebuffer_width;
            dim._current_frame_height   = framebuffer_height;
            
            INFOLOG("W: " << a_width << " H: " << a_height);
            INFOLOG("fW: " << framebuffer_width << " fH: " << framebuffer_height);
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

            int32 key       = static_cast<int32>(a_key);
            int32 scancode  = static_cast<int32>(a_scancode);
            int32 action    = static_cast<int32>(a_action);
            int32 modifier  = static_cast<int32>(a_mods);

            ApplicationInstance::ProvideInputManagment()->SendKeyEvent(user_data->_id, key, scancode, action, modifier);
        }

        void glfw_char_callback(GLFWwindow* a_window, unsigned int a_char)
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            uint16 character = static_cast<uint16>(a_char);

            ApplicationInstance::ProvideInputManagment()->SendCharEvent(user_data->_id, character);
        }

        void glfw_mousebutton_callback(GLFWwindow* a_window, int a_key, int a_action, int a_mods) 
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            int32 key       = static_cast<int32>(a_key);
            int32 action    = static_cast<int32>(a_action);
            int32 modifier  = static_cast<int32>(a_mods);
            int32 undefined = -1;

            ApplicationInstance::ProvideInputManagment()->SendMouseEvent(user_data->_id, key, undefined, action, modifier);
        }

        void glfw_mouse_callback(GLFWwindow* a_window, double a_x_pos, double a_y_pos) 
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            float32 x_pos = static_cast<float32>(a_x_pos);
            float32 y_pos = static_cast<float32>(a_y_pos);

            ApplicationInstance::ProvideInputManagment()->SendDirectionalEvent(user_data->_id, x_pos, y_pos);
        }

        void glfw_scroll_callback(GLFWwindow* a_window, double a_x_offset, double a_y_offset) 
        {
            WindowInstance* user_data = reinterpret_cast<WindowInstance*>(glfwGetWindowUserPointer(a_window));

            float32 x_offset = static_cast<float32>(a_x_offset);
            float32 y_offset = static_cast<float32>(a_y_offset);

            ApplicationInstance::ProvideInputManagment()->SendScrollEvent(user_data->_id, x_offset, y_offset);
        }

    } // End of namespace ~ GLFWWindowCallBacks

    WindowDimension::WindowDimension()
        : _current_width(1280)
        , _current_height(720)
        , _current_frame_height(1280)
        , _current_frame_width(720)
        , _current_x_pos(0)
        , _current_y_pos(0)
    {
    }

    WindowInstance::WindowInstance()  
        : _id(DUIDGenerator::GenerateID())
        , _window()
        , _name("")
        , _should_be_closed(false)
        , _is_iconified(false)
        , _is_focussed(false)
    {
    }

    WindowManagementSystem::WindowManagementSystem(const std::string a_main_window_name)
        :   _default_width(1280),
            _default_height(720),
            _default_window_name(a_main_window_name)
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
    }

    void WindowManagementSystem::TerminateWindowManagement()
    {
        bgfx::shutdown();
        glfwTerminate();
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

        ApplicationInstance::ProvideInputManagment()->RegisterWindow(new_window._id);

        GLFWwindow* glfw_window = glfwCreateWindow(a_width, a_height, a_name, NULL, NULL);
        new_window._window      = glfw_window;
        new_window._name        = a_name;

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

        auto& [it_pair, result] =_window_instances.emplace(new_window._id, new_window);
        if (result)
        {
            return &(it_pair->second);
        }
        else
        {
            ERRORLOG("Window Management System couldn't construct a new window.");
            return nullptr;
        }
    }

    void WindowManagementSystem::DestructWindow(std::string a_name)
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