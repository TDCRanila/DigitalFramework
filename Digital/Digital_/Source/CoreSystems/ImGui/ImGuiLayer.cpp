#include <CoreSystems/ImGui/ImGuiLayer.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Rendering.h>
#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Window.h>
#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Utility.h>
#include <CoreSystems/Window/WindowManagement.h>

namespace DFW
{
	void ImGuiLayer::InitImGuiLayer()
	{
		// Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Config
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Enable Docking Feature
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Enable Multi-Viewport Feature
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		SharedPtr<DWindow::WindowInstance> main_window_ptr	= CoreService::GetWindowSystem()->GetMainWindow();
		GLFWwindow* glfw_window_ptr							= reinterpret_cast<GLFWwindow*>(CoreService::GetWindowSystem()->GetMainWindowNWH());
		DFW_ASSERT(main_window_ptr, "Pointer to the main window is invalid, window management might not have been initialised.");

		io.DisplaySize	= ImVec2(
				static_cast<float>(main_window_ptr->_window_dimension._current_width)
			,	static_cast<float>(main_window_ptr->_window_dimension._current_height)
		);

		io.DeltaTime	= 1.0f / 60.0f;
		// TODO ImGui .ini filepath.
		io.IniFilename	= NULL;

		// Style
		SetupStyle();

		/// Render & Platform Initialization and binding.
		
		// TODO Implement other window API & implement compiler defines.
		ImGui_ImplGlfw_InitForOther(glfw_window_ptr, true);

		DImGui::ImGui_ImplBGFX_InitWindowPlatform(glfw_window_ptr);
		DImGui::ImGui_ImplBGFX_InitGraphics();

		// Custom Callbacks.
		// Clipboard
	}

	void ImGuiLayer::TerminateImGuiLayer()
	{
		// Render & Platform Shutdown
		DImGui::ImGui_ImplBGFX_ShutdownGraphics();
		DImGui::ImGui_ImplBGFX_ShutdownWindowPlatform();

		// Context
		ImGui::DestroyContext();

		DImGui::imgui_rendering_context = DImGui::ImGuiRenderingContext();
	}

	void ImGuiLayer::BeginFrame(float32 const a_delta_time)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = a_delta_time;

		DImGui::ImGui_ImplBGFX_NewFrameGraphics();
		DImGui::ImGui_ImplBGFX_NewFrameWindow();

		ImGui::NewFrame();
	}

	void ImGuiLayer::EndFrame()
	{
		// TODO Debug: Software Cursor
		ImGuiIO& const io = ImGui::GetIO();
		ImGui::GetForegroundDrawList()->AddCircleFilled(io.MousePos, 4.0f, 0xFFFFFFFF);
		
		Debug_DrawViewportRenderingInformation();

		ImGui::Render();

		DImGui::ImGui_ImplBGFX_RenderDrawData(ImGui::GetMainViewport()->DrawData, DImGui::main_window_id);
		
		// Update and Render additional Viewports / Platform Windows
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

		ImVec4 const bgColor			= ColourFromBytes(30, 30, 31);
		ImVec4 const lightBgColor		= ColourFromBytes(72, 72, 75);
		ImVec4 const veryLightBgColor	= ColourFromBytes(80, 80, 85);
					 
		ImVec4 const panelColor			= ColourFromBytes(41, 41, 45);
		ImVec4 const panelHoverColor	= ColourFromBytes(245, 80, 0);
		ImVec4 const panelActiveColor	= ColourFromBytes(190, 55, 10);
					 
		ImVec4 const textColor			= ColourFromBytes(255, 255, 255);
		ImVec4 const textDisabledColor	= ColourFromBytes(151, 151, 151);
		ImVec4 const borderColor		= ColourFromBytes(78, 78, 78);

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

	void ImGuiLayer::Debug_DrawViewportRenderingInformation() const
	{
		ImGuiIO& const io = ImGui::GetIO();
		ImGui::Text("DisplaySize: [%f | %f]", io.DisplaySize.x, io.DisplaySize.y);
		ImGui::Text("DisplayFramebufferScale: [%f | %f]", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui::Text("[%f | %f]", io.MousePos.x, io.MousePos.y);

		for (auto const& viewport_data : DImGui::imgui_rendering_context._viewports)
		{
			ImGuiViewport const* viewport = ImGui::FindViewportByPlatformHandle(viewport_data->_window);
			if (viewport)
			{
				ImGui::Text("-----");
				ImGui::Text("Viewport ID: [%d]", static_cast<uint16>(viewport_data->_view_id));
				ImGui::Text("Framebuffer ID: [%d]", static_cast<uint16>(viewport_data->_framebuffer_handle.idx));
				ImGui::Text("DisplaySize: [%f | %f]", viewport->Size.x, viewport->Size.y);
			}
		}
	}

} // End of namespace ~ DFW.
