#pragma once

#include <CoreSystems/Window/WindowManagement.h>
#include <CoreSystems/Window/WindowData.h>


// Forward Declare(s)
struct GLFWwindow;

namespace DFW
{
    // FW Declare.
    class ApplicationInstance;

    namespace DWindow
    {
        struct WindowInstanceGLFW : public WindowInstance
        {
            GLFWwindow* _glfw_window;
        };

        class WindowManagementGLFW final : public WindowManagement
        {
            friend ApplicationInstance;
        public:
            WindowManagementGLFW() = default;
            virtual ~WindowManagementGLFW() = default;

            virtual SharedPtr<WindowInstance> ConstructWindow(WindowParameters const& a_window_parameters) override;
            virtual void RequestWindowClose(WindowID const a_window_id) override;
            virtual void DestroyWindowsRequestedForClosure() override;

            // Native Window Handle, e.g. GLFWindow*;
            virtual void* GetWindowNWH(WindowID const a_window_id) const override;
            // Platform Window Handle, e.g. HWND;
            virtual void* GetWindowPWH(WindowID const a_window_id) const override;

            virtual void ChangeWindowParameters(WindowID const a_window_id, WindowParameters const& a_window_parameters) override;

        private:
            virtual void InitWindowManagement() override;
            virtual void TerminateWindowManagement() override;
            virtual void PollWindowEvents() override;

        };

    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.