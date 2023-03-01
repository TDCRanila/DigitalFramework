#pragma once

#include <CoreSystems/Window/WindowManagement.h>
#include <CoreSystems/Window/WindowData.h>


// Forward Declare(s)
struct GLFWwindow;

namespace DFW
{
    namespace DWindow
    {
        struct WindowInstanceGLFW : public WindowInstance
        {
            GLFWwindow* _glfw_window;
        };

        class WindowManagementGLFW final : public WindowManagement
        {
        public:
            WindowManagementGLFW() = default;
            virtual ~WindowManagementGLFW() = default;

            virtual void Init() override;
            virtual void Terminate() override;
            virtual void PollWindowEvents() override;

        public:
            virtual SharedPtr<WindowInstance> ConstructWindow(WindowParameters const& a_window_parameters) override;
            virtual void RequestWindowClose(WindowID const a_window_id) override;
            virtual void DestroyWindowsRequestedForClosure() override;

            // Native Window Handle, e.g. GLFWindow*;
            virtual void* GetWindowNWH(WindowID const a_window_id) const override;
            // Platform Window Handle, e.g. HWND;
            virtual void* GetWindowPWH(WindowID const a_window_id) const override;

            virtual void ChangeWindowParameters(WindowID const a_window_id, WindowParameters const& a_window_parameters) override;

            virtual void RequestMouseCursorCapture() override;
            virtual void RequestMouseCursorRelease() override;

        };

    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.