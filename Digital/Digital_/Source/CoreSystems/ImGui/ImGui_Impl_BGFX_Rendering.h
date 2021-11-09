#pragma once

#include <bgfx/bgfx.h>

// FW Declare
struct ImDrawData;
struct ImGuiViewport;

namespace DImGui
{
	// Backend API
	bool ImGui_ImplBGFX_InitGraphics();
	void ImGui_ImplBGFX_ShutdownGraphics();
	void ImGui_ImplBGFX_NewFrameGraphics();
	void ImGui_ImplBGFX_RenderDrawData(ImDrawData const* const a_draw_data, bgfx::ViewId const a_view_id);

	// Viewport - Renderer callback declarations 
	void ImGui_ImplBGFX_RenderViewportWindow(ImGuiViewport* a_viewport, void*);
	void ImGui_ImplBGFX_SwapBuffers(ImGuiViewport* a_viewport, void*);

	// (Optional) Called by Init/NewFrame/Shutdown
	bool ImGui_ImplBGFX_CreateFontsTexture();
	void ImGui_ImplBGFX_DestroyFontsTexture();
	bool ImGui_ImplBGFX_CreateDeviceObjects();
	void ImGui_ImplBGFX_DestroyDeviceObjects();



} // End of namespace ~ DImGui.
