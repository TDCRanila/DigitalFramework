#pragma once

#include <CoreSystems/Window/WindowData.h>
#include <CoreSystems/ApplicationEvents.h>
#include <CoreSystems/Memory.h>

// Forward Declare(s)
struct GLFWwindow;

namespace DFW
{
    namespace DWindow
    {
        constexpr const char*   DFW_DEFAULT_WINDOW_NAME     = "DIGITAL";
        constexpr int32         DFW_DEFAULT_WINDOW_WIDTH    = 1280;
        constexpr int32         DFW_DEFAULT_WINDOW_HEIGHT   = 720;

        using WindowContainer = std::unordered_map<WindowID, SharedPtr<WindowInstance>>;

        class WindowManagement
        {
        public:
            WindowManagement() = default;
            virtual ~WindowManagement() = default;

            static SharedPtr<WindowManagement> Construct();

            virtual void Init() = 0;
            virtual void Terminate() = 0;
            virtual void PollWindowEvents() = 0;

        public:
            virtual SharedPtr<WindowInstance> ConstructWindow(WindowParameters const& a_window_parameters) = 0;
            virtual void RequestWindowClose(WindowID const a_window_id) = 0;
            virtual void DestroyWindowsRequestedForClosure() = 0;
            bool HaveAllWindowsBeenClosed() const;

            WindowID GetMainWindowID() const;
            SharedPtr<WindowInstance> const GetMainWindow() const;
            SharedPtr<WindowInstance> const GetFocussedWindow() const;
            SharedPtr<WindowInstance> const GetWindow(WindowID const a_window_id) const;
            SharedPtr<WindowInstance> const GetWindow(std::string const& a_window_name) const;
            
            // Native Window Handle, e.g. GLFWindow*;
            virtual void* GetWindowNWH(WindowID const a_window_id) const = 0;
            void* GetMainWindowNWH();
            // Platform Window Handle, e.g. HWND;
            virtual void* GetWindowPWH(WindowID const a_window_id) const = 0;
            void* GetMainWindowPWH();
            
            bool IsWindowFocussed(WindowID const a_window_id) const;
            bool IsWindowMinimized(WindowID const a_window_id) const;

            virtual void ChangeWindowParameters(WindowID const a_window_id, WindowParameters const& a_window_parameters) = 0;

            virtual void RequestMouseCursorCapture() = 0;
            virtual void RequestMouseCursorRelease() = 0;
            
            WindowParameters default_window_parameters;

        protected:
            SharedPtr<WindowInstance>& GetWindowInternal(WindowID const a_window_id);

            void RegisterCommonEventCallbacks();
            void UnregisterCommonEventCallbacks();

            WindowContainer _window_instances;
            std::vector<WindowID> _windows_requested_destruction;
            WindowID _main_window_id;
            WindowID _focussed_window_id;
        
        private:
            void OnWindowFocusEvent(WindowFocusEvent const& a_event);

        };


    } // End of namespace ~ DWindow.

} // End of namespace ~ DFW.
