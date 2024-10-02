#include <DFW/CoreSystems/Window/WindowManagement.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <DFW/CoreSystems/Window/Windows/WindowManagementGLFW.h>
#include <DFW/CoreSystems/Events/EventDispatcher.h>
#include <DFW/CoreSystems/Input/InputManagement.h>
#include <DFW/CoreSystems/Logging/Logger.h>
#include <DFW/CoreSystems/CoreServices.h>

#include <DFW/Defines/Defines.h>


namespace DFW
{
    namespace DWindow
    {
        SharedPtr<WindowManagement> WindowManagement::Construct()
        {
            // TODO Implement Platform IfDefs
            return MakeShared<WindowManagementGLFW>();
        }

        WindowID WindowManagement::GetMainWindowID() const
        {
            return _main_window_id;
        }

        SharedPtr<WindowInstance> const WindowManagement::GetMainWindow() const
        {
            return _window_instances.at(_main_window_id);
        }

        SharedPtr<WindowInstance> const WindowManagement::GetFocussedWindow() const
        {
            return GetWindow(_focussed_window_id);
        }

        SharedPtr<WindowInstance> const WindowManagement::GetWindow(WindowID const a_window_id) const
        {
            if (auto const it = _window_instances.find(a_window_id); it != _window_instances.end())
            {
                return (*it).second;
            }
            else
            {
                DFW_WARNLOG("Attempting to find a window with ID: {}, but it cannot be found.", a_window_id);
                return SharedPtr<WindowInstance>();
            }
        }

        void* WindowManagement::GetMainWindowNWH()
        {
            return GetWindowNWH(_main_window_id);
        }

        void* WindowManagement::GetMainWindowPWH()
        {
            return GetWindowPWH(_main_window_id);
        }

        bool WindowManagement::HaveAllWindowsBeenClosed() const
        {
            return _window_instances.size() <= 0;
        }

        bool WindowManagement::IsWindowFocussed(WindowID const a_window_id) const
        {
            return GetWindow(a_window_id)->is_focussed;
        }

        bool WindowManagement::IsWindowMinimized(WindowID const a_window_id) const
        {
            return GetWindow(a_window_id)->is_minimized;
        }

        SharedPtr<WindowInstance>& WindowManagement::GetWindowInternal(WindowID const a_window_id)
        {
            auto const it = _window_instances.find(a_window_id);
            if (it == _window_instances.end())
            {
                DFW_ERRORLOG("Attempting to find a window with ID: {}, but it cannot be found.", a_window_id);
                DFW_ASSERT((*it).second && "Attempting to find a window, but it cannot be found.");
            }

            return (*it).second;
        }

        void WindowManagement::RegisterCommonEventCallbacks()
        {
            CoreService::GetAppEventHandler()->RegisterCallback<WindowFocusEvent, &WindowManagement::OnWindowFocusEvent>(this);
        }

        void WindowManagement::UnregisterCommonEventCallbacks()
        {
            CoreService::GetAppEventHandler()->UnregisterCallback<WindowFocusEvent, &WindowManagement::OnWindowFocusEvent>(this);
        }

        void WindowManagement::OnWindowFocusEvent(WindowFocusEvent const& a_event)
        {
            _focussed_window_id = a_event.window_id;
        }

    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
