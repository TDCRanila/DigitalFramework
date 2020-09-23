#pragma once

#include <bgfx/bgfx.h>

#include <imgui/imgui.h>
#include <imgui/imgui_user.h>

#include <CoreSystems/InputManagement.h> 
#include <CoreSystems/WindowManagement.h>

//#include <imgui/resources/icons_kenney.h>
//#include <imgui/resources/icons_font_awesome.h>

namespace bx 
{ 
	struct AllocatorI; 
} // End of namespace ~ bx.

namespace DCore
{
	struct OcornutImguiContext;

	class ImGuiLayer
	{
	public:
		ImGuiLayer() = default;

		void CreateContext(float32 a_fontSize = 18.0f, bx::AllocatorI* a_allocator = NULL);
		void DestroyContext();

		void BeginFrame(const DCore::InputData& a_input_data, const DCore::WindowDimension& a_window_dimension, bgfx::ViewId a_view_id = 255);
		void EndFrame();
	
		static OcornutImguiContext _imgui_context;

	};

	struct OcornutImguiContext
	{
		void Create(float32 a_fontSize, bx::AllocatorI* a_allocator);
		void Destroy();

		void Render(ImDrawData* a_draw_data);

		void BeginFrame(const DCore::InputData& a_input_data, const DCore::WindowDimension& a_window_dimension, bgfx::ViewId a_viewId);
		void EndFrame();

		void SetupStyle();

		ImGuiContext*		_imgui_context;
		bx::AllocatorI*		_allocator;

		bgfx::VertexLayout  _layout;
		
		bgfx::ProgramHandle _program;
		bgfx::ProgramHandle _image_program;
		
		bgfx::ViewId		_view_id;

		bgfx::TextureHandle _texture_handle;
		bgfx::UniformHandle s_tex;
		bgfx::UniformHandle u_imageLodEnabled;
		
		ImFont*				_font[::ImGui::Font::Count];
		
		int64				_last;
		int32				_last_scroll;
		
	};

} // End of namespace ~ DCore.

namespace ImGui
{

#define IMGUI_FLAGS_NONE        UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

	ImTextureID ToID(bgfx::TextureHandle a_handle, uint8 a_flags, uint8 a_mip);

	// Helper function for passing bgfx::TextureHandle to ImGui::Image.
	void Image(bgfx::TextureHandle a_handle
		, uint8 a_flags
		, uint8 a_mip
		, const ImVec2& a_size
		, const ImVec2& a_uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& a_uv1 = ImVec2(1.0f, 1.0f)
		, const ImVec4& a_tint_colour	= ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
		, const ImVec4& a_border_colour = ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	// Helper function for passing bgfx::TextureHandle to ImGui::Image.
	void Image(bgfx::TextureHandle a_handle
		, const ImVec2& a_size
		, const ImVec2& a_uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& a_uv1 = ImVec2(1.0f, 1.0f)
		, const ImVec4& a_tint_colour	= ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
		, const ImVec4& a_border_colour = ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	// Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
	bool ImageButton(bgfx::TextureHandle a_handle
		, uint8 a_flags
		, uint8 a_mip
		, const ImVec2& a_size
		, const ImVec2& a_uv0	= ImVec2(0.0f, 0.0f)
		, const ImVec2& a_uv1	= ImVec2(1.0f, 1.0f)
		, int32 a_frame_padding = -1
		, const ImVec4& a_backgroundg_colour	= ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		, const ImVec4& a_tint_colour			= ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
	bool ImageButton(bgfx::TextureHandle a_handle
		, const ImVec2& a_size
		, const ImVec2& a_uv0	= ImVec2(0.0f, 0.0f)
		, const ImVec2& a_uv1	= ImVec2(1.0f, 1.0f)
		, int32 a_frame_padding = -1
		, const ImVec4& a_backgroundg_colour	= ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		, const ImVec4& a_tint_colour			= ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

} // End of namespace ~ ImGui.
