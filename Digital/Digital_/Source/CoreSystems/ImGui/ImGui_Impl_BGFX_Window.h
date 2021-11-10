#pragma once

#include <imgui/imgui.h>

struct GLFWwindow;
struct GLFWmonitor;

namespace DFW
{
	namespace DImGui
	{
		bool ImGui_ImplBGFX_InitWindowPlatform(GLFWwindow* a_window);
		void ImGui_ImplBGFX_ShutdownWindowPlatform();
		void ImGui_ImplBGFX_NewFrameWindow();

		// Viewport - Helper Functions
		void ImGui_ImplBGFX_InitPlatformInterface();
		void ImGui_ImplBGFX_ShutdownPlatformInterface();
		void ImGui_ImplBGFX_CreateViewportWindow(ImGuiViewport* a_viewport);
		void ImGui_ImplBGFX_DestroyViewportWindow(ImGuiViewport* a_viewport);
		void ImGui_ImplBGFX_UpdateViewPortWindow(ImGuiViewport* a_viewport);

		namespace ImGuiPlatformInterfaceHelpers
		{
			void ImGui_ImplBGFX_ShowWindow(ImGuiViewport* a_viewport);
			void ImGui_ImplBGFX_SetWindowTitle(ImGuiViewport* a_viewport, const char* a_title);

			void ImGui_ImplBGFX_SetWindowPos(ImGuiViewport* a_viewport, ImVec2 a_pos);
			ImVec2 ImGui_ImplBGFX_GetWindowPos(ImGuiViewport* a_viewport);

			void ImGui_ImplBGFX_SetWindowSize(ImGuiViewport* a_viewport, ImVec2 a_size);
			ImVec2 ImGui_ImplBGFX_GetWindowSize(ImGuiViewport* a_viewport);

			void ImGui_ImplBGFX_SetWindowFocus(ImGuiViewport* a_viewport);
			bool ImGui_ImplBGFX_GetWindowFocus(ImGuiViewport* a_viewport);
			bool ImGui_ImplBGFX_GetWindowMinimized(ImGuiViewport* a_viewport);

		} // End of namespace ~ ImGuiPlatoformInterfaceHelpers

		namespace ImGuiGLFWCallbacks
		{
			void ImGui_ImplBGFX_WindowCloseCallback(GLFWwindow* a_window);
			void ImGui_ImplBGFX_WindowPosCallback(GLFWwindow* a_window, int, int);
			void ImGui_ImplBGFX_WindowSizeCallback(GLFWwindow* a_window, int, int);

			void ImGui_ImplBGFX_MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods);
			void ImGui_ImplBGFX_ScrollCallback(GLFWwindow* a_window, double a_xoffset, double a_yoffset);
			void ImGui_ImplBGFX_KeyCallback(GLFWwindow* a_window, int a_key, int scancode, int action, int mods);
			void ImGui_ImplBGFX_CharCallback(GLFWwindow* a_window, unsigned int a_c);
			void ImGui_ImplBGFX_MonitorCallback(GLFWmonitor* a_monitor, int a_event);
		} // End of namespace ~ ImGuiGLFWCallbacks

	} // End of namespace ~ DImGui

} // End of namespace ~ DFW.
