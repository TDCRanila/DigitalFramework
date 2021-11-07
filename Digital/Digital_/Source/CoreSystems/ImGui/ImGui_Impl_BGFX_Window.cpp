#include <CoreSystems/Imgui/ImGui_Impl_BGFX_Window.h>

#include <bgfx/embedded_shader.h>
#include <bx/bx.h>
#include <bx/allocator.h>
#include <bx/math.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <imgui/backends/imgui_impl_glfw.h>

#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Utility.h>
#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Rendering.h>


namespace DImGui
{
	bool ImGui_ImplBGFX_InitWindow(GLFWwindow* window, bool install_callbacks)
	{
        DImGui::main_window = window;

        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        main_viewport->PlatformHandle = (void*)window;

#ifdef _WIN32
        main_viewport->PlatformHandleRaw = glfwGetWin32Window(window);
#endif

        // Set ImGui IOs.
        ImGuiIO& io = ImGui::GetIO();

        std::string const backend_platform_name("GLFW");
        io.BackendPlatformName  = backend_platform_name.c_str();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
            io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
            ImGui_ImplBGFX_InitPlatformInterface();
		}

#ifdef IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
        if (g_GlVersion >= 320)
            io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
#endif

        // TODO - proper return and checking if something can go wrong
        return true;
	}

	void ImGui_ImplBGFX_ShutdownWindow()
	{
		//ImGui_ImplBGFX_ShutdownPlatformInterface();

		ImGui::DestroyContext(DImGui::imgui_context._imgui_context);

		DImGui::imgui_context._allocator = NULL;
	}

	void ImGui_ImplBGFX_NewFrameWindow()
	{
        // Resize Framebuffer Windows.

		ImGui_ImplGlfw_NewFrame();
		ImGuiIO& io = ImGui::GetIO();
	}

    void ImGui_ImplBGFX_InitPlatformInterface()
    {
        // Register platform interface (will be coupled with a renderer interface)
        ImGuiPlatformIO& platform_io            = ImGui::GetPlatformIO();
        platform_io.Platform_CreateWindow       = ImGui_ImplBGFX_CreateWindow;
        platform_io.Platform_DestroyWindow      = ImGui_ImplBGFX_DestroyWindow;
        platform_io.Platform_RenderWindow       = ImGui_ImplBGFX_RenderWindow;
        platform_io.Platform_SwapBuffers        = ImGui_ImplBGFX_SwapBuffers;

        platform_io.Platform_ShowWindow         = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_ShowWindow;
        platform_io.Platform_SetWindowPos       = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_SetWindowPos;
        platform_io.Platform_GetWindowPos       = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_GetWindowPos;
        platform_io.Platform_SetWindowSize      = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_SetWindowSize;
        platform_io.Platform_GetWindowSize      = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_GetWindowSize;
        platform_io.Platform_SetWindowFocus     = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_SetWindowFocus;
        platform_io.Platform_GetWindowFocus     = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_GetWindowFocus;
        platform_io.Platform_GetWindowMinimized = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_GetWindowMinimized;
        platform_io.Platform_SetWindowTitle     = ImGuiPlatoformInterfaceHelpers::ImGui_ImplBGFX_SetWindowTitle;

#if GLFW_HAS_WINDOW_ALPHA
        platform_io.Platform_SetWindowAlpha = ImGui_ImplGlfw_SetWindowAlpha;
#endif

#if HAS_WIN32_IME
        platform_io.Platform_SetImeInputPos = ImGui_ImplWin32_SetImeInputPos;
#endif

        // Register main window handle (which is owned by the main application, not by us)
        // This is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.) can use same logic for main and secondary viewports.
        ImGuiViewport* main_viewport    = ImGui::GetMainViewport();
        ImGuiViewportDataBGFX* data     = IM_NEW(ImGuiViewportDataBGFX)();

        data->_window                   = DImGui::main_window;
        data->_window_owned             = false;

        main_viewport->PlatformUserData = data;
        main_viewport->PlatformHandle   = (void*)DImGui::main_window;
    }

	void ImGui_ImplBGFX_CreateWindow(ImGuiViewport* viewport)
	{
        ImGuiViewportDataBGFX* data = IM_NEW(ImGuiViewportDataBGFX)();

        // TODO CHANGE
        static int32 counter = 1;
        data->_view_id = counter;
        ++counter;
        viewport->PlatformUserData = data;

        // GLFW 3.2 unfortunately always set focus on glfwCreateWindow() if GLFW_VISIBLE is set, regardless of GLFW_FOCUSED
        // With GLFW 3.3, the hint GLFW_FOCUS_ON_SHOW fixes this problem
        glfwWindowHint(GLFW_VISIBLE, false);
        glfwWindowHint(GLFW_FOCUSED, false);
#if GLFW_HAS_FOCUS_ON_SHOW
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, false);
#endif
        // TODO READD
        //glfwWindowHint(GLFW_DECORATED, (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? false : true);
#if GLFW_HAS_WINDOW_TOPMOST
        glfwWindowHint(GLFW_FLOATING, (viewport->Flags & ImGuiViewportFlags_TopMost) ? true : false);
#endif

        data->_window = glfwCreateWindow((int)viewport->Size.x, (int)viewport->Size.y, "No Title Yet", NULL, NULL);
        //data->_window = glfwCreateWindow(0, 0, "No Title Yet", NULL, NULL);
        data->_window_owned = true;
        viewport->PlatformHandle = (void*)data->_window;

#ifdef _WIN32
        viewport->PlatformHandleRaw = glfwGetWin32Window(data->_window);
        DImGui::imgui_context._framebuffer_handles[data->_view_id] = bgfx::createFrameBuffer(viewport->PlatformHandleRaw, uint16_t(viewport->Size.x), uint16_t(viewport->Size.y));
        //DImGui::imgui_context._framebuffer_handles[data->_view_id] = bgfx::createFrameBuffer(viewport->PlatformHandleRaw, 1280, 720);
        
#endif

        glfwSetWindowPos(data->_window, (int)viewport->Pos.x, (int)viewport->Pos.y);

        // Install GLFW callbacks for secondary viewports
        glfwSetMouseButtonCallback(data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_MouseButtonCallback);
        glfwSetScrollCallback(data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_ScrollCallback);
        glfwSetKeyCallback(data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_KeyCallback);
        glfwSetCharCallback(data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_CharCallback);

        glfwSetWindowCloseCallback(data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_WindowCloseCallback);
        glfwSetWindowPosCallback(data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_WindowPosCallback);
        glfwSetWindowSizeCallback(data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_WindowSizeCallback);
	}

	void ImGui_ImplBGFX_DestroyWindow(ImGuiViewport* viewport)
	{
        if (ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData)
        {
            if (data->_window_owned)
            {
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED && defined(_WIN32)
                HWND hwnd = (HWND)viewport->PlatformHandleRaw;
                ::RemovePropA(hwnd, "IMGUI_VIEWPORT");
#endif
                glfwDestroyWindow(data->_window);
            }
            data->_window = NULL;
            IM_DELETE(data);
        }
        viewport->PlatformUserData = viewport->PlatformHandle = NULL;
	}
	
    namespace ImGuiPlatoformInterfaceHelpers
    {
        void ImGui_ImplBGFX_ShowWindow(ImGuiViewport* viewport)
        {
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;

#if defined(_WIN32)
            // GLFW hack: Hide icon from task bar
            HWND hwnd = (HWND)viewport->PlatformHandleRaw;
            if (viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
            {
                LONG ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
                ex_style &= ~WS_EX_APPWINDOW;
                ex_style |= WS_EX_TOOLWINDOW;
                ::SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);
            }

            // GLFW hack: install hook for WM_NCHITTEST message handler
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED && defined(_WIN32)
            ::SetPropA(hwnd, "IMGUI_VIEWPORT", viewport);
            if (g_GlfwWndProc == NULL)
                g_GlfwWndProc = (WNDPROC)::GetWindowLongPtr(hwnd, GWLP_WNDPROC);
            ::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndProcNoInputs);
#endif

#if !GLFW_HAS_FOCUS_ON_SHOW
            // GLFW hack: GLFW 3.2 has a bug where glfwShowWindow() also activates/focus the window.
            // The fix was pushed to GLFW repository on 2018/01/09 and should be included in GLFW 3.3 via a GLFW_FOCUS_ON_SHOW window attribute.
            // See https://github.com/glfw/glfw/issues/1189
            // FIXME-VIEWPORT: Implement same work-around for Linux/OSX in the meanwhile.
            if (viewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing)
            {
                ::ShowWindow(hwnd, SW_SHOWNA);
                return;
            }
#endif
#endif

            glfwShowWindow(data->_window);
        }

        void ImGui_ImplBGFX_SetWindowTitle(ImGuiViewport* viewport, const char* title)
        {
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
            glfwSetWindowTitle(data->_window, title);
        }

        void ImGui_ImplBGFX_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos)
        {
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
            data->_ignore_window_pos_event_frame = ImGui::GetFrameCount();
            glfwSetWindowPos(data->_window, (int)pos.x, (int)pos.y);
        }

        ImVec2 ImGui_ImplBGFX_GetWindowPos(ImGuiViewport* viewport)
        {
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
            int x = 0, y = 0;
            glfwGetWindowPos(data->_window, &x, &y);
            return ImVec2((float)x, (float)y);
        }

        void ImGui_ImplBGFX_SetWindowSize(ImGuiViewport* viewport, ImVec2 size)
        {
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
#if __APPLE__ && !GLFW_HAS_OSX_WINDOW_POS_FIX
            // Native OS windows are positioned from the bottom-left corner on macOS, whereas on other platforms they are
            // positioned from the upper-left corner. GLFW makes an effort to convert macOS style coordinates, however it
            // doesn't handle it when changing size. We are manually moving the window in order for changes of size to be based
            // on the upper-left corner.
            int x, y, width, height;
            glfwGetWindowPos(data->_window, &x, &y);
            glfwGetWindowSize(data->_window, &width, &height);
            glfwSetWindowPos(data->_window, x, y - height + size.y);
#endif
            DImGui::imgui_context._framebuffer_handles[data->_view_id] = bgfx::createFrameBuffer(viewport->PlatformHandleRaw, uint16_t(viewport->Size.x), uint16_t(viewport->Size.y));

            data->_ignore_window_size_event_frame = ImGui::GetFrameCount();
            glfwSetWindowSize(data->_window, (int)size.x, (int)size.y);
        }

        ImVec2 ImGui_ImplBGFX_GetWindowSize(ImGuiViewport* viewport)
        {
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
            int w = 0, h = 0;
            glfwGetWindowSize(data->_window, &w, &h);
            return ImVec2((float)w, (float)h);
        }

        void ImGui_ImplBGFX_SetWindowFocus(ImGuiViewport* viewport)
        {
#if GLFW_HAS_FOCUS_WINDOW
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
            glfwFocusWindow(data->_window);
#else
            // FIXME: What are the effect of not having this function? At the moment imgui doesn't actually call SetWindowFocus - we set that up ahead, will answer that question later.
            (void)viewport;
#endif
        }

        bool ImGui_ImplBGFX_GetWindowFocus(ImGuiViewport* viewport)
        {
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
            return glfwGetWindowAttrib(data->_window, GLFW_FOCUSED) != 0;
        }

        bool ImGui_ImplBGFX_GetWindowMinimized(ImGuiViewport* viewport)
        {
            ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
            return glfwGetWindowAttrib(data->_window, GLFW_ICONIFIED) != 0;
        }

    } // End of namespace ~ ImGuiPlatoformInterfaceHelpers

    namespace ImGuiGLFWCallbacks
    {
        void ImGui_ImplBGFX_WindowCloseCallback(GLFWwindow* window)
        {
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
                viewport->PlatformRequestClose = true;
        }

        void ImGui_ImplBGFX_WindowPosCallback(GLFWwindow* window, int, int)
        {
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
            {
                if (ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData)
                {
                    bool ignore_event = (ImGui::GetFrameCount() <= data->_ignore_window_pos_event_frame + 1);
                    //data->IgnoreWindowPosEventFrame = -1;
                    if (ignore_event)
                        return;
                }
                viewport->PlatformRequestMove = true;
            }
        }

        void ImGui_ImplBGFX_WindowSizeCallback(GLFWwindow* window, int, int)
        {
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
            {
                if (ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData)
                {
                    bool ignore_event = (ImGui::GetFrameCount() <= data->_ignore_window_size_event_frame + 1);
                    //data->IgnoreWindowSizeEventFrame = -1;
                    if (ignore_event)
                        return;
                }
                viewport->PlatformRequestResize = true;
            }
        }

        void ImGui_ImplBGFX_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
        {
            if (DImGui::prev_user_callback_mousebutton!= NULL && window == DImGui::main_window)
                DImGui::prev_user_callback_mousebutton(window, button, action, mods);

            if (action == GLFW_PRESS && button >= 0 && button < IM_ARRAYSIZE(DImGui::mouse_just_pressed))
                DImGui::mouse_just_pressed[button] = true;
        }

        void ImGui_ImplBGFX_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
        {
            if (DImGui::prev_user_callback_scroll != NULL && window == DImGui::main_window)
                DImGui::prev_user_callback_scroll(window, xoffset, yoffset);

            ImGuiIO& io = ImGui::GetIO();
            io.MouseWheelH += (float)xoffset;
            io.MouseWheel += (float)yoffset;
        }

        void ImGui_ImplBGFX_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (DImGui::prev_user_callback_key != NULL && window == DImGui::main_window)
                DImGui::prev_user_callback_key(window, key, scancode, action, mods);

            ImGuiIO& io = ImGui::GetIO();
            if (action == GLFW_PRESS)
                io.KeysDown[key] = true;
            if (action == GLFW_RELEASE)
                io.KeysDown[key] = false;

            // Modifiers are not reliable across systems
            io.KeyCtrl  = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
            io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT]  || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
            io.KeyAlt   = io.KeysDown[GLFW_KEY_LEFT_ALT]      || io.KeysDown[GLFW_KEY_RIGHT_ALT];
#ifdef _WIN32
            io.KeySuper = false;
#else
            io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
        }

        void ImGui_ImplBGFX_CharCallback(GLFWwindow* window, unsigned int c)
        {
            if (DImGui::prev_user_callback_char != NULL && window == DImGui::main_window)
                DImGui::prev_user_callback_char(window, c);

            ImGuiIO& io = ImGui::GetIO();
            io.AddInputCharacter(c);
        }

        void ImGui_ImplBGFX_MonitorCallback(GLFWmonitor*, int)
        {
            DImGui::want_update_monitors = true;
        }

    } // End of namespace ~ ImGuiGLFWCallbacks.

} // End of namespace ~ DImGui