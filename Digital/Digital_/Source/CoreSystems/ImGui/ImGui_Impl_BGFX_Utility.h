#pragma once

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>

#include <imgui/imgui.h>
#include <imgui-user/imgui_user.h>

#include <imgui-resources/vs_ocornut_imgui.bin.h>
#include <imgui-resources/fs_ocornut_imgui.bin.h>
#include <imgui-resources/vs_imgui_image.bin.h>
#include <imgui-resources/fs_imgui_image.bin.h>

#include <imgui-resources/roboto_regular.ttf.h>
#include <imgui-resources/robotomono_regular.ttf.h>
#include <imgui-resources/icons_kenney.ttf.h>
#include <imgui-resources/icons_font_awesome.ttf.h>

#include <glfw/glfw3.h>

#include <vector>
#include <array>

namespace DImGui
{
	// Helper structure we store in the void* RenderUserData field of each ImGuiViewport to easily retrieve our backend data.
	struct ImGuiViewportDataBGFX
	{
		ImGuiViewportDataBGFX();

		GLFWwindow*				_window;
		bgfx::ViewId			_view_id;

		bgfx::FrameBufferHandle _framebuffer_handle;
		int32					_ignore_window_pos_event_frame;
		int32					_ignore_window_size_event_frame;
		bool					_window_owned;
	};
	
	struct ImGuiRenderingContext
	{
		ImGuiRenderingContext();

		std::vector<ImGuiViewportDataBGFX*> _viewports;

		std::array<ImFont*, ::ImGui::Font::Count> _font;

		bgfx::VertexLayout	_vertex_layout;
		bgfx::ProgramHandle	_shader_program_handle;
		bgfx::ProgramHandle	_image_program_handle;
		bgfx::TextureHandle	_texture_handle;
		bgfx::UniformHandle	_texture_uniform_handle;
		bgfx::UniformHandle	_image_lod_uniform_handle;

		std::vector<bgfx::ViewId>	_free_viewport_ids;
		uint32						_viewport_id_counter;

		bool						_are_graphic_devices_initialized;
	};

	bgfx::ViewId ImGui_ImplBGFX_AllocateViewportID();
	void ImGui_ImplBGFX_FreeViewportID(bgfx::ViewId const a_viewport_id);

	extern ImGuiRenderingContext	imgui_rendering_context;
	extern GLFWwindow*				main_window;
	extern bgfx::ViewId				main_window_id;

	extern bool					mouse_just_pressed[ImGuiMouseButton_COUNT];
	extern bool                 installed_callbacks;
	extern bool                 want_update_monitors;

	extern GLFWmousebuttonfun   prev_user_callback_mousebutton;
	extern GLFWscrollfun        prev_user_callback_scroll;
	extern GLFWkeyfun           prev_user_callback_key;
	extern GLFWcharfun          prev_user_callback_char;
	extern GLFWmonitorfun       prev_user_callback_monitor;

	static const bgfx::EmbeddedShader embedded_imgui_shaders[] =
	{
		BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
		BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
		BGFX_EMBEDDED_SHADER(vs_imgui_image),
		BGFX_EMBEDDED_SHADER(fs_imgui_image),

		BGFX_EMBEDDED_SHADER_END()
	};

#define ICON_MIN_KI 0xe900
#define ICON_MAX_KI 0xe9e3

#define ICON_MIN_FA 0xf000
#define ICON_MAX_FA 0xf2e0

	struct FontRangeMerge
	{
		const void* data;
		size_t      size;
		ImWchar     ranges[3];
	};

	static FontRangeMerge s_fontRangeMerge[] =
	{
		{ s_iconsKenneyTtf,      sizeof(s_iconsKenneyTtf),      { ICON_MIN_KI, ICON_MAX_KI, 0 } },
		{ s_iconsFontAwesomeTtf, sizeof(s_iconsFontAwesomeTtf), { ICON_MIN_FA, ICON_MAX_FA, 0 } },
	};

} // End of namespace ~ DImGui.

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
		, const ImVec4& a_tint_colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
		, const ImVec4& a_border_colour = ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	// Helper function for passing bgfx::TextureHandle to ImGui::Image.
	void Image(bgfx::TextureHandle a_handle
		, const ImVec2& a_size
		, const ImVec2& a_uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& a_uv1 = ImVec2(1.0f, 1.0f)
		, const ImVec4& a_tint_colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
		, const ImVec4& a_border_colour = ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	// Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
	bool ImageButton(bgfx::TextureHandle a_handle
		, uint8 a_flags
		, uint8 a_mip
		, const ImVec2& a_size
		, const ImVec2& a_uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& a_uv1 = ImVec2(1.0f, 1.0f)
		, int32 a_frame_padding = -1
		, const ImVec4& a_backgroundg_colour = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		, const ImVec4& a_tint_colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
	bool ImageButton(bgfx::TextureHandle a_handle
		, const ImVec2& a_size
		, const ImVec2& a_uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& a_uv1 = ImVec2(1.0f, 1.0f)
		, int32 a_frame_padding = -1
		, const ImVec4& a_backgroundg_colour = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		, const ImVec4& a_tint_colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

} // End of namespace ~ ImGui.
