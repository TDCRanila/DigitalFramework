#pragma once

#include <imgui/imgui.h>

struct GLFWwindow;
struct GLFWmonitor;

namespace DImGui
{
	bool ImGui_ImplBGFX_InitWindow(GLFWwindow* window, bool install_callbacks);
	void ImGui_ImplBGFX_ShutdownWindow();
	void ImGui_ImplBGFX_NewFrameWindow();

	void ImGui_ImplBGFX_InitPlatformInterface();

	// Viewport - Helper Functions
	void ImGui_ImplBGFX_CreateWindow(ImGuiViewport* viewport);
	void ImGui_ImplBGFX_DestroyWindow(ImGuiViewport* viewport);
	
	namespace ImGuiPlatoformInterfaceHelpers
	{
		void ImGui_ImplBGFX_ShowWindow(ImGuiViewport* viewport);
		void ImGui_ImplBGFX_SetWindowTitle(ImGuiViewport* viewport, const char* title);
		
		void ImGui_ImplBGFX_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos);
		ImVec2 ImGui_ImplBGFX_GetWindowPos(ImGuiViewport* viewport);
		
		void ImGui_ImplBGFX_SetWindowSize(ImGuiViewport* viewport, ImVec2 size);
		ImVec2 ImGui_ImplBGFX_GetWindowSize(ImGuiViewport* viewport);
		
		void ImGui_ImplBGFX_SetWindowFocus(ImGuiViewport* viewport);
		bool ImGui_ImplBGFX_GetWindowFocus(ImGuiViewport* viewport);
		bool ImGui_ImplBGFX_GetWindowMinimized(ImGuiViewport* viewport);

	} // End of namespace ~ ImGuiPlatoformInterfaceHelpers

	// Window - Renderer callback declarations 
	namespace ImGuiGLFWCallbacks
	{
		void ImGui_ImplBGFX_WindowCloseCallback(GLFWwindow* window);
		void ImGui_ImplBGFX_WindowPosCallback(GLFWwindow* window, int, int);
		void ImGui_ImplBGFX_WindowSizeCallback(GLFWwindow* window, int, int);

		void ImGui_ImplBGFX_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		void ImGui_ImplBGFX_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void ImGui_ImplBGFX_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void ImGui_ImplBGFX_CharCallback(GLFWwindow* window, unsigned int c);
		void ImGui_ImplBGFX_MonitorCallback(GLFWmonitor* monitor, int event);
	} // End of namespace ~ ImGuiGLFWCallbacks


} // End of namespace ~ DImGui
