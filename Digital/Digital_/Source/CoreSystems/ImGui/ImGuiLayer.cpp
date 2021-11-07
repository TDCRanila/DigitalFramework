#include <CoreSystems/ImGui/ImGuiLayer.h>

// TODO CAN WE PREVENT THESE INCLUDES
#include <GLFW/glfw3.h>

#include <bgfx/bgfx.h>
#include <bx/allocator.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Rendering.h>
#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Window.h>
#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Utility.h>

namespace DCore
{

	void ImGuiLayer::InitImGuiLayer(const WindowInstance& a_window_instance)
	{
		// Allocator

		// Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Enable Multi-Viewport / Platform Windows
		// TODO: Need to investigate viewport issues.
		// Disabled for now as it causes alginment issues regarding the mouse cursor.
		// Possibly to do with bgfx rendering, or the imgui translation of the windows.
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		const WindowDimension& dim = a_window_instance._window_dimension;
		io.DisplaySize	= ImVec2(static_cast<float>(dim._current_width), static_cast<float>(dim._current_height));
		io.DeltaTime	= 1.0f / 60.0f;
		io.IniFilename	= NULL;

		// Debug IO
		//io.MouseDrawCursor = true;

		// Style
		SetupStyle();

		// Render & Platform Initialization and binding.

		// TODO: Simply using the imgui ogl example for buttons etc.
		// Should eventually be all moved to BGFX Window and handle input there as well.
		ImGui_ImplGlfw_InitForOpenGL(a_window_instance._window, true);

		DImGui::ImGui_ImplBGFX_InitWindow(a_window_instance._window, false);
		
		//ImGui_ImplBGFX_Init(a_window_instance._window);
		DImGui::ImGui_ImplBGFX_InitGraphics();

		// Custom Callbacks.
	
		// Clipboard
		
		// Add Fonts
		DImGui::ImGui_ImplBGFX_AddFonts();
	}

	void ImGuiLayer::TerminateImGuiLayer()
	{
		// Render & Platform Shutdown

		// Context
		ImGui::DestroyContext();

	}

	void ImGuiLayer::BeginFrame(const DCore::InputData& a_input_data, const DCore::WindowDimension& a_window_dimension)
	{


		// TODO Remove?
		UNUSED(a_input_data);

		ImGuiIO& io		= ImGui::GetIO();
		
		// TODO Get DeltaTime, thorugh core service
		// io.DeltaTime = 

		// TODO Use event to change this instead of constant.
		io.DisplaySize	= ImVec2(static_cast<float>(a_window_dimension._current_width), static_cast<float>(a_window_dimension._current_height));

		DImGui::ImGui_ImplBGFX_NewFrameGraphics();

		DImGui::ImGui_ImplBGFX_NewFrameWindow();

		ImGui::NewFrame();
	}

	void ImGuiLayer::EndFrame()
	{
		ImGuiIO& const io = ImGui::GetIO();
						
		// TODO Debug: Software Cursor
		ImGui::GetForegroundDrawList()->AddCircleFilled(io.MousePos, 4.0f, 0xFFFFFFFF);
		
		ImGui::Text("DisplaySize: [%f | %f]", io.DisplaySize.x, io.DisplaySize.y);
		ImGui::Text("DisplayFramebufferScale: [%f | %f]", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui::Text("[%f | %f]", io.MousePos.x, io.MousePos.y);
		
		ImGui::Render();

		// Render Draw Data
		bgfx::ViewId main_window = 0;

		// Set view 0 to the same dimensions as the window and to clear the color buffer.
		//bgfx::setViewClear(main_window, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00cc6600);
		//bgfx::setViewRect(main_window, 0, 0, bgfx::BackbufferRatio::Equal);

		DImGui::ImGui_ImplBGFX_RenderDrawData(ImGui::GetDrawData(), main_window);
		
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

	}

	void ImGuiLayer::SetupStyle()
	{
		// TODO: add Invert/Light Version

		// Style: https://github.com/ocornut/imgui/issues/707#issuecomment-670976818
		constexpr auto ColourFromBytes = [](uint8 r, uint8 g, uint8 b)
		{
			return ImVec4(static_cast<float32>(r) / 255.0f, static_cast<float32>(g) / 255.0f, static_cast<float32>(b) / 255.0f, 1.0f);
		};

		ImGuiStyle& style	= ImGui::GetStyle();
		ImVec4* colours		= style.Colors;

		const ImVec4 bgColor			= ColourFromBytes(30, 30, 31);
		const ImVec4 lightBgColor		= ColourFromBytes(72, 72, 75);
		const ImVec4 veryLightBgColor	= ColourFromBytes(80, 80, 85);

		const ImVec4 panelColor			= ColourFromBytes(41, 41, 45);
		const ImVec4 panelHoverColor	= ColourFromBytes(245, 80, 0);
		const ImVec4 panelActiveColor	= ColourFromBytes(190, 55, 10);

		const ImVec4 textColor			= ColourFromBytes(255, 255, 255);
		const ImVec4 textDisabledColor	= ColourFromBytes(151, 151, 151);
		const ImVec4 borderColor		= ColourFromBytes(78, 78, 78);

		colours[ImGuiCol_Text]					= textColor;
		colours[ImGuiCol_TextDisabled]			= textDisabledColor;
		colours[ImGuiCol_TextSelectedBg]		= panelActiveColor;
		colours[ImGuiCol_WindowBg]				= bgColor;
		colours[ImGuiCol_ChildBg]				= bgColor;
		colours[ImGuiCol_PopupBg]				= bgColor;
		colours[ImGuiCol_Border]				= borderColor;
		colours[ImGuiCol_BorderShadow]			= borderColor;
		colours[ImGuiCol_FrameBg]				= panelColor;
		colours[ImGuiCol_FrameBgHovered]		= panelHoverColor;
		colours[ImGuiCol_FrameBgActive]			= panelActiveColor;
		colours[ImGuiCol_TitleBg]				= bgColor;
		colours[ImGuiCol_TitleBgActive]			= bgColor;
		colours[ImGuiCol_TitleBgCollapsed]		= bgColor;
		colours[ImGuiCol_MenuBarBg]				= panelColor;
		colours[ImGuiCol_ScrollbarBg]			= panelColor;
		colours[ImGuiCol_ScrollbarGrab]			= lightBgColor;
		colours[ImGuiCol_ScrollbarGrabHovered]	= veryLightBgColor;
		colours[ImGuiCol_ScrollbarGrabActive]	= veryLightBgColor;
		colours[ImGuiCol_CheckMark]				= panelActiveColor;
		colours[ImGuiCol_SliderGrab]			= panelHoverColor;
		colours[ImGuiCol_SliderGrabActive]		= panelActiveColor;
		colours[ImGuiCol_Button]				= panelColor;
		colours[ImGuiCol_ButtonHovered]			= panelHoverColor;
		colours[ImGuiCol_ButtonActive]			= panelHoverColor;
		colours[ImGuiCol_Header]				= panelColor;
		colours[ImGuiCol_HeaderHovered]			= panelHoverColor;
		colours[ImGuiCol_HeaderActive]			= panelActiveColor;
		colours[ImGuiCol_Separator]				= borderColor;
		colours[ImGuiCol_SeparatorHovered]		= borderColor;
		colours[ImGuiCol_SeparatorActive]		= borderColor;
		colours[ImGuiCol_ResizeGrip]			= bgColor;
		colours[ImGuiCol_ResizeGripHovered]		= panelColor;
		colours[ImGuiCol_ResizeGripActive]		= lightBgColor;
		colours[ImGuiCol_PlotLines]				= panelActiveColor;
		colours[ImGuiCol_PlotLinesHovered]		= panelHoverColor;
		colours[ImGuiCol_PlotHistogram]			= panelActiveColor;
		colours[ImGuiCol_PlotHistogramHovered]	= panelHoverColor;
		colours[ImGuiCol_ModalWindowDimBg]		= bgColor;
		colours[ImGuiCol_DragDropTarget]		= bgColor;
		colours[ImGuiCol_NavHighlight]			= bgColor;
		//colours[ImGuiCol_DockingPreview]		= panelActiveColor;
		colours[ImGuiCol_Tab]					= bgColor;
		colours[ImGuiCol_TabActive]				= panelActiveColor;
		colours[ImGuiCol_TabUnfocused]			= bgColor;
		colours[ImGuiCol_TabUnfocusedActive]	= panelActiveColor;
		colours[ImGuiCol_TabHovered]			= panelHoverColor;

		style.WindowRounding	= 0.0f;
		style.ChildRounding		= 0.0f;
		style.FrameRounding		= 0.0f;
		style.GrabRounding		= 0.0f;
		style.PopupRounding		= 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.TabRounding		= 0.0f;

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	}

} // End of namespace ~ DCore.
