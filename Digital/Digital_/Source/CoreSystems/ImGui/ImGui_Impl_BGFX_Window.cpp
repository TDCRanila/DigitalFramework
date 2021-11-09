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
	bool ImGui_ImplBGFX_InitWindowPlatform(GLFWwindow* a_window)
	{
        DImGui::main_window = a_window;

        ImGuiViewport* main_viewport    = ImGui::GetMainViewport();
        main_viewport->PlatformHandle   = static_cast<void*>(a_window);

#ifdef _WIN32
        main_viewport->PlatformHandleRaw = glfwGetWin32Window(a_window);
#else
#error "Unsupported Platform."
#endif

        // Set ImGui IOs.
        ImGuiIO& io = ImGui::GetIO();

        std::string const backend_platform_name("GLFW");
        io.BackendPlatformName  = backend_platform_name.c_str();

        // Enable Viewport Decoration.
        io.ConfigViewportsNoDecoration = false;

        // Setup Viewports interface if the bitflag is enabled.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
            io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
            ImGui_ImplBGFX_InitPlatformInterface();
		}

#ifdef IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
        if (g_GlVersion >= 320)
            io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
#endif

        return true;
	}

	void ImGui_ImplBGFX_ShutdownWindowPlatform()
	{
		ImGui_ImplBGFX_ShutdownPlatformInterface();

        for (DImGui::ImGuiViewportDataBGFX const* const viewport_data : DImGui::imgui_rendering_context._viewports)
        {
            bgfx::destroy(viewport_data->_framebuffer_handle);
        }
	}

    void ImGui_ImplBGFX_NewFrameWindow()
    {
        ImGui_ImplGlfw_NewFrame();
    }

    void ImGui_ImplBGFX_InitPlatformInterface()
    {
        // Register platform interface (will be coupled with a renderer interface)
        ImGuiPlatformIO& platform_io            = ImGui::GetPlatformIO();
        platform_io.Platform_CreateWindow       = ImGui_ImplBGFX_CreateViewportWindow;
        platform_io.Platform_DestroyWindow      = ImGui_ImplBGFX_DestroyViewportWindow;
        platform_io.Platform_RenderWindow       = ImGui_ImplBGFX_RenderViewportWindow;
        platform_io.Platform_UpdateWindow       = ImGui_ImplBGFX_UpdateViewPortWindow;
        platform_io.Platform_SwapBuffers        = ImGui_ImplBGFX_SwapBuffers;

        platform_io.Platform_ShowWindow         = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_ShowWindow;
        platform_io.Platform_SetWindowPos       = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_SetWindowPos;
        platform_io.Platform_GetWindowPos       = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_GetWindowPos;
        platform_io.Platform_SetWindowSize      = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_SetWindowSize;
        platform_io.Platform_GetWindowSize      = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_GetWindowSize;
        platform_io.Platform_SetWindowFocus     = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_SetWindowFocus;
        platform_io.Platform_GetWindowFocus     = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_GetWindowFocus;
        platform_io.Platform_GetWindowMinimized = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_GetWindowMinimized;
        platform_io.Platform_SetWindowTitle     = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_SetWindowTitle;

#if GLFW_HAS_WINDOW_ALPHA
        platform_io.Platform_SetWindowAlpha = ImGui_ImplGlfw_SetWindowAlpha;
#endif

#if HAS_WIN32_IME
        platform_io.Platform_SetImeInputPos = ImGui_ImplWin32_SetImeInputPos;
#endif

        // Register main window handle (which is owned by the main application, not by us)
        // This is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.) can use same logic for main and secondary viewports.
        ImGuiViewportDataBGFX* main_viewport_data   = DImGui::imgui_rendering_context._viewports.emplace_back(IM_NEW(ImGuiViewportDataBGFX)());
        main_viewport_data->_view_id                = ImGui_ImplBGFX_AllocateViewportID();
        main_window_id                              = main_viewport_data->_view_id;
        main_viewport_data->_window                 = DImGui::main_window;
        main_viewport_data->_window_owned           = false;

        ImGuiViewport* main_viewport                = ImGui::GetMainViewport();
        main_viewport->PlatformUserData             = main_viewport_data;
        main_viewport->PlatformHandle               = static_cast<void*>(DImGui::main_window);
    }

    void ImGui_ImplBGFX_ShutdownPlatformInterface()
    {
        // Unregister platform interface
        ImGuiPlatformIO& platform_io            = ImGui::GetPlatformIO();
        platform_io.Platform_CreateWindow       = nullptr;
        platform_io.Platform_DestroyWindow      = nullptr;
        platform_io.Platform_RenderWindow       = nullptr;
        platform_io.Platform_SwapBuffers        = nullptr;
                                                         
        platform_io.Platform_ShowWindow         = nullptr;
        platform_io.Platform_SetWindowPos       = nullptr;
        platform_io.Platform_GetWindowPos       = nullptr;
        platform_io.Platform_SetWindowSize      = nullptr;
        platform_io.Platform_GetWindowSize      = nullptr;
        platform_io.Platform_SetWindowFocus     = nullptr;
        platform_io.Platform_GetWindowFocus     = nullptr;
        platform_io.Platform_GetWindowMinimized = nullptr;
        platform_io.Platform_SetWindowTitle     = nullptr;
    }

	void ImGui_ImplBGFX_CreateViewportWindow(ImGuiViewport* a_viewport)
	{
        ImGuiViewportDataBGFX* viewport_data = DImGui::imgui_rendering_context._viewports.emplace_back(IM_NEW(ImGuiViewportDataBGFX)());
        viewport_data->_view_id = ImGui_ImplBGFX_AllocateViewportID();
                
        // GLFW 3.2 unfortunately always set focus on glfwCreateWindow() if GLFW_VISIBLE is set, regardless of GLFW_FOCUSED
        // With GLFW 3.3, the hint GLFW_FOCUS_ON_SHOW fixes this problem
        glfwWindowHint(GLFW_VISIBLE, false);
        glfwWindowHint(GLFW_FOCUSED, false);
#if GLFW_HAS_FOCUS_ON_SHOW
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, false);
#endif

        glfwWindowHint(GLFW_DECORATED, (a_viewport->Flags & ImGuiViewportFlags_NoDecoration) ? false : true);
#if GLFW_HAS_WINDOW_TOPMOST
        glfwWindowHint(GLFW_FLOATING, (viewport->Flags & ImGuiViewportFlags_TopMost) ? true : false);
#endif

        viewport_data->_window = glfwCreateWindow(
            static_cast<int>(a_viewport->Size.x), 
            static_cast<int>(a_viewport->Size.y), 
            "No Title Yet", 
            NULL, 
            NULL
        );

        viewport_data->_window_owned    = true;
        a_viewport->PlatformHandle      = static_cast<void*>(viewport_data->_window);
        a_viewport->PlatformUserData    = viewport_data;

#ifdef _WIN32
        a_viewport->PlatformHandleRaw   = glfwGetWin32Window(viewport_data->_window);
        
        if (bgfx::isValid(viewport_data->_framebuffer_handle))
            bgfx::destroy(viewport_data->_framebuffer_handle);

        viewport_data->_framebuffer_handle = bgfx::createFrameBuffer(
                    a_viewport->PlatformHandleRaw
                ,   static_cast<uint16>(a_viewport->Size.x)
                ,   static_cast<uint16>(a_viewport->Size.y)
            );
        
#else
#error "Unsupported Platform"
#endif

        glfwSetWindowPos(viewport_data->_window, static_cast<int>(a_viewport->Pos.x), static_cast<int>(a_viewport->Pos.y));

        // Install GLFW callbacks for secondary viewports
        glfwSetMouseButtonCallback(viewport_data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_MouseButtonCallback);
        glfwSetScrollCallback(viewport_data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_ScrollCallback);
        glfwSetKeyCallback(viewport_data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_KeyCallback);
        glfwSetCharCallback(viewport_data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_CharCallback);

        glfwSetWindowCloseCallback(viewport_data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_WindowCloseCallback);
        glfwSetWindowPosCallback(viewport_data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_WindowPosCallback);
        glfwSetWindowSizeCallback(viewport_data->_window, ImGuiGLFWCallbacks::ImGui_ImplBGFX_WindowSizeCallback);
	}

	void ImGui_ImplBGFX_DestroyViewportWindow(ImGuiViewport* a_viewport)
	{
        if (ImGuiViewportDataBGFX* viewport_data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData))
        {
            auto& viewport_vec = DImGui::imgui_rendering_context._viewports;
            if (auto it = std::find(viewport_vec.begin(), viewport_vec.end(), viewport_data); it != viewport_vec.end())
                viewport_vec.erase(it);
            else
                DFW_ASSERT(false, "Attempting to destroy viewport window, but not registered.");

            bgfx::destroy(viewport_data->_framebuffer_handle);

            ImGui_ImplBGFX_FreeViewportID(viewport_data->_view_id);

            if (viewport_data->_window_owned)
            {
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED && defined(_WIN32)
                HWND hwnd = (HWND)viewport->PlatformHandleRaw;
                ::RemovePropA(hwnd, "IMGUI_VIEWPORT");
#endif
                glfwDestroyWindow(viewport_data->_window);
            }
            viewport_data->_window = nullptr;
            IM_DELETE(viewport_data);
        }
        a_viewport->PlatformUserData = a_viewport->PlatformHandle = nullptr;
	}
	
    void ImGui_ImplBGFX_UpdateViewPortWindow(ImGuiViewport* a_viewport)
    {
        // Manually set the viewport/window size when the OS/Platform has authoration over the size changes,
        // as ImGui doesn't do that internally.
        if (!(a_viewport->Flags & ImGuiViewportFlags_::ImGuiViewportFlags_NoDecoration) && a_viewport->PlatformRequestResize)
        {
            ImVec2 const window_size = ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_GetWindowSize(a_viewport);
            ImGuiPlatformInterfaceHelpers::ImGui_ImplBGFX_SetWindowSize(a_viewport, window_size);
        }
    }

    namespace ImGuiPlatformInterfaceHelpers
    {
        void ImGui_ImplBGFX_ShowWindow(ImGuiViewport* a_viewport)
        {
            ImGuiViewportDataBGFX* data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);

#if defined(_WIN32)
            // GLFW hack: Hide icon from task bar
            HWND hwnd = static_cast<HWND>(a_viewport->PlatformHandleRaw);
            if (a_viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
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
            if (a_viewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing)
            {
                ::ShowWindow(hwnd, SW_SHOWNA);
                return;
            }
#endif
#endif

            glfwShowWindow(data->_window);
        }

        void ImGui_ImplBGFX_SetWindowTitle(ImGuiViewport* a_viewport, const char* a_title)
        {
            ImGuiViewportDataBGFX* data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);
            glfwSetWindowTitle(data->_window, a_title);
        }

        void ImGui_ImplBGFX_SetWindowPos(ImGuiViewport* a_viewport, ImVec2 a_pos)
        {
            ImGuiViewportDataBGFX* data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);
            data->_ignore_window_pos_event_frame = ImGui::GetFrameCount();
            glfwSetWindowPos(data->_window, static_cast<int>(a_pos.x), static_cast<int>(a_pos.y));
        }

        ImVec2 ImGui_ImplBGFX_GetWindowPos(ImGuiViewport* a_viewport)
        {
            ImGuiViewportDataBGFX const* data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);
            int x = 0, y = 0;
            glfwGetWindowPos(data->_window, &x, &y);
            return ImVec2(static_cast<float>(x), static_cast<float>(y));
        }

        void ImGui_ImplBGFX_SetWindowSize(ImGuiViewport* a_viewport, ImVec2 a_size)
        {
            ImGuiViewportDataBGFX* viewport_data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);
#if __APPLE__ && !GLFW_HAS_OSX_WINDOW_POS_FIX
            // Native OS windows are positioned from the bottom-left corner on macOS, whereas on other platforms they are
            // positioned from the upper-left corner. GLFW makes an effort to convert macOS style coordinates, however it
            // doesn't handle it when changing size. We are manually moving the window in order for changes of size to be based
            // on the upper-left corner.
            int x, y, width, height;
            glfwGetWindowPos(viewport_data->_window, &x, &y);
            glfwGetWindowSize(viewport_data->_window, &width, &height);
            glfwSetWindowPos(viewport_data->_window, x, y - height + size.y);
#endif
            bgfx::destroy(viewport_data->_framebuffer_handle);
            viewport_data->_framebuffer_handle = bgfx::createFrameBuffer(
                    a_viewport->PlatformHandleRaw
                , static_cast<uint16>(a_viewport->Size.x)
                , static_cast<uint16>(a_viewport->Size.y)
            );

            viewport_data->_ignore_window_size_event_frame = ImGui::GetFrameCount();
            glfwSetWindowSize(viewport_data->_window, static_cast<int>(a_size.x), static_cast<int>(a_size.y));
        }

        ImVec2 ImGui_ImplBGFX_GetWindowSize(ImGuiViewport* a_viewport)
        {
            ImGuiViewportDataBGFX const* data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);
            int w = 0, h = 0;
            glfwGetWindowSize(data->_window, &w, &h);
            return ImVec2(static_cast<float>(w), static_cast<float>(h));
        }

        void ImGui_ImplBGFX_SetWindowFocus(ImGuiViewport* a_viewport)
        {
#if GLFW_HAS_FOCUS_WINDOW
            ImGuiViewportDataBGFX* data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);
            glfwFocusWindow(data->_window);
#else
            // FIXME: What are the effect of not having this function? At the moment imgui doesn't actually call SetWindowFocus - we set that up ahead, will answer that question later.
            (void)a_viewport;
#endif
        }

        bool ImGui_ImplBGFX_GetWindowFocus(ImGuiViewport* a_viewport)
        {
            ImGuiViewportDataBGFX const* data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);
            return glfwGetWindowAttrib(data->_window, GLFW_FOCUSED) != 0;
        }

        bool ImGui_ImplBGFX_GetWindowMinimized(ImGuiViewport* a_viewport)
        {
            ImGuiViewportDataBGFX const* data = static_cast<ImGuiViewportDataBGFX*>(a_viewport->PlatformUserData);
            return glfwGetWindowAttrib(data->_window, GLFW_ICONIFIED) != 0;
        }

    } // End of namespace ~ ImGuiPlatoformInterfaceHelpers

    namespace ImGuiGLFWCallbacks
    {
        void ImGui_ImplBGFX_WindowCloseCallback(GLFWwindow* a_window)
        {
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(a_window))
                viewport->PlatformRequestClose = true;
        }

        void ImGui_ImplBGFX_WindowPosCallback(GLFWwindow* a_window, int, int)
        {
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(a_window))
            {
                if (ImGuiViewportDataBGFX const* data = static_cast<ImGuiViewportDataBGFX*>(viewport->PlatformUserData))
                {
                    bool ignore_event = (ImGui::GetFrameCount() <= data->_ignore_window_pos_event_frame + 1);
                    //data->IgnoreWindowPosEventFrame = -1;
                    if (ignore_event)
                        return;
                }
                viewport->PlatformRequestMove = true;
            }
        }

        void ImGui_ImplBGFX_WindowSizeCallback(GLFWwindow* a_window, int, int)
        {
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(a_window))
            {
                if (ImGuiViewportDataBGFX const* data = static_cast<ImGuiViewportDataBGFX*>(viewport->PlatformUserData))
                {
                    bool ignore_event = (ImGui::GetFrameCount() <= data->_ignore_window_size_event_frame + 1);
                    //data->IgnoreWindowSizeEventFrame = -1;
                    if (ignore_event)
                        return;
                }
                viewport->PlatformRequestResize = true;
            }
        }

        void ImGui_ImplBGFX_MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
        {
            if (DImGui::prev_user_callback_mousebutton!= NULL && a_window == DImGui::main_window)
                DImGui::prev_user_callback_mousebutton(a_window, a_button, a_action, a_mods);

            if (a_action == GLFW_PRESS && a_button >= 0 && a_button < IM_ARRAYSIZE(DImGui::mouse_just_pressed))
                DImGui::mouse_just_pressed[a_button] = true;
        }

        void ImGui_ImplBGFX_ScrollCallback(GLFWwindow* a_window, double a_xoffset, double a_yoffset)
        {
            if (DImGui::prev_user_callback_scroll != NULL && a_window == DImGui::main_window)
                DImGui::prev_user_callback_scroll(a_window, a_xoffset, a_yoffset);

            ImGuiIO& io = ImGui::GetIO();
            io.MouseWheelH  += static_cast<float>(a_xoffset);
            io.MouseWheel   += static_cast<float>(a_yoffset);
        }

        void ImGui_ImplBGFX_KeyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
        {
            if (DImGui::prev_user_callback_key != NULL && a_window == DImGui::main_window)
                DImGui::prev_user_callback_key(a_window, a_key, a_scancode, a_action, a_mods);

            ImGuiIO& io = ImGui::GetIO();
            if (a_action == GLFW_PRESS)
                io.KeysDown[a_key] = true;
            if (a_action == GLFW_RELEASE)
                io.KeysDown[a_key] = false;

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

        void ImGui_ImplBGFX_CharCallback(GLFWwindow* a_window, unsigned int a_c)
        {
            if (DImGui::prev_user_callback_char != NULL && a_window == DImGui::main_window)
                DImGui::prev_user_callback_char(a_window, a_c);

            ImGuiIO& io = ImGui::GetIO();
            io.AddInputCharacter(a_c);
        }

        void ImGui_ImplBGFX_MonitorCallback(GLFWmonitor*, int)
        {
            DImGui::want_update_monitors = true;
        }

    } // End of namespace ~ ImGuiGLFWCallbacks.

} // End of namespace ~ DImGui
