#include <CoreSystems/Window/WindowManagement.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Logging/Logger.h>
#include <Defines/Defines.h>

#include <CoreSystems/Window/Windows/WindowManagementGLFW.h>

namespace DFW
{
    namespace DWindow
    {
        SharedPtr<WindowManagementSystem> WindowManagementSystem::Construct()
        {
            // TODO Implement Platform IfDefs
            return MakeShared<WindowManagementGLFW>();
        }

        WindowID WindowManagementSystem::GetMainWindowID() const
        {
            return _main_window_id;
        }

        SharedPtr<WindowInstance> const WindowManagementSystem::GetMainWindow() const
        {
            return _window_instances.at(_main_window_id);
        }

        SharedPtr<WindowInstance> const WindowManagementSystem::GetFocussedWindow() const
        {
            return GetWindow(_focussed_window_id);
        }

        SharedPtr<WindowInstance> const WindowManagementSystem::GetWindow(WindowID const a_window_id) const
        {
            if (auto const it = _window_instances.find(a_window_id); it != _window_instances.end())
            {
                return (*it).second;
            }
            else
            {
                DFW_ERRORLOG("Attempting to find a window with ID: {}, but it cannot be found.", a_window_id);
                return SharedPtr<WindowInstance>();
            }
        }

        void* WindowManagementSystem::GetMainWindowNWH()
        {
            return GetWindowNWH(_main_window_id);
        }

        void* WindowManagementSystem::GetMainWindowPWH()
        {
            return GetWindowPWH(_main_window_id);
        }

        bool WindowManagementSystem::HaveAllWindowsBeenClosed() const
        {
            return _window_instances.size() <= 0;
        }

        bool WindowManagementSystem::IsWindowFocussed(WindowID const a_window_id) const
        {
            return GetWindow(a_window_id)->is_focussed;
        }

        bool WindowManagementSystem::IsWindowMinimized(WindowID const a_window_id) const
        {
            return GetWindow(a_window_id)->is_minimized;
        }

        SharedPtr<WindowInstance>& WindowManagementSystem::GetWindowInternal(WindowID const a_window_id)
        {
            if (auto const it = _window_instances.find(a_window_id); it != _window_instances.end())
            {
                return (*it).second;
            }
            else
            {
                DFW_ERRORLOG("Attempting to find a window with ID: {}, but it cannot be found.", a_window_id);
                DFW_ASSERT((*it).second);
                SharedPtr<WindowInstance> ptr;
                return ptr;
            }
        }

        void WindowManagementSystem::RegisterCommonEventCallbacks()
        {
            CoreService::GetMainEventHandler()->RegisterCallback<WindowFocusEvent, &WindowManagementSystem::OnWindowFocusEvent>(this);
        }

        void WindowManagementSystem::UnregisterCommonEventCallbacks()
        {
            CoreService::GetMainEventHandler()->UnregisterCallback<WindowFocusEvent, &WindowManagementSystem::OnWindowFocusEvent>(this);
        }

        void WindowManagementSystem::OnWindowFocusEvent(WindowFocusEvent const& a_event)
        {
            _focussed_window_id = a_event.window_id;
        }

    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
