#pragma once

#include <imgui/imgui.h>

namespace bgfx
{
	typedef uint16_t ViewId;
} // End of namespace ~ bgfx.

namespace DImGui
{
	// Backend API
	bool ImGui_ImplBGFX_InitGraphics();
	void ImGui_ImplBGFX_ShutdownGraphics();
	void ImGui_ImplBGFX_NewFrameGraphics();
	void ImGui_ImplBGFX_RenderDrawData(ImDrawData* draw_data, bgfx::ViewId a_view_id = 255);

	// Viewport - Renderer callback declarations 
	void ImGui_ImplBGFX_RenderWindow(ImGuiViewport* viewport, void*);
	void ImGui_ImplBGFX_SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height, float& a_ortho_return);
	void ImGui_ImplBGFX_SwapBuffers(ImGuiViewport* viewport, void*);

	// (Optional) Called by Init/NewFrame/Shutdown
	bool ImGui_ImplBGFX_CreateFontsTexture();
	void ImGui_ImplBGFX_DestroyFontsTexture();
	bool ImGui_ImplBGFX_CreateDeviceObjects();
	void ImGui_ImplBGFX_DestroyDeviceObjects();



} // End of namespace ~ DImGui.
