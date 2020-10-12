#pragma once

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/bx.h>
#include <bx/allocator.h>

#include <imgui/imgui.h>
#include <imgui/imgui_user.h>

#include <imgui/resources/vs_ocornut_imgui.bin.h>
#include <imgui/resources/fs_ocornut_imgui.bin.h>
#include <imgui/resources/vs_imgui_image.bin.h>
#include <imgui/resources/fs_imgui_image.bin.h>

#include <imgui/resources/roboto_regular.ttf.h>
#include <imgui/resources/robotomono_regular.ttf.h>
#include <imgui/resources/icons_kenney.ttf.h>
#include <imgui/resources/icons_font_awesome.ttf.h>

#include <glfw/glfw3.h>

struct GLFWwindow;

namespace DImGui
{

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

	// Helper structure we store in the void* RenderUserData field of each ImGuiViewport to easily retrieve our backend data.
	struct ImGuiViewportDataBGFX
	{
		GLFWwindow*		_window;
		bool			_window_owned;
		int32			_ignore_window_pos_event_frame;
		int32			_ignore_window_size_event_frame;
		bgfx::ViewId	_view_id;

		ImGuiViewportDataBGFX();
		~ImGuiViewportDataBGFX();
	};

	struct OcornutImguiContext
	{
		ImGuiContext* _imgui_context;
		bx::AllocatorI* _allocator;

		bgfx::FrameBufferHandle _framebuffer_handles[256];

		bgfx::VertexLayout  _layouts[256];

		bgfx::ProgramHandle _programs[256];
		bgfx::ProgramHandle _image_programs[256];

		bgfx::TextureHandle _texture_handles[256];
		bgfx::UniformHandle s_texs[256];
		bgfx::UniformHandle u_imageLodEnabled[256];

		ImFont* _font[::ImGui::Font::Count];

		int64				_last;
		int32				_last_scroll;
	};

	extern OcornutImguiContext	imgui_context;
	extern GLFWwindow*			main_window;

	extern bool					mouse_just_pressed[ImGuiMouseButton_COUNT];
	extern bool                 installed_callbacks;
	extern bool                 want_update_monitors;

	extern GLFWmousebuttonfun   prev_user_callback_mousebutton;
	extern GLFWscrollfun        prev_user_callback_scroll;
	extern GLFWkeyfun           prev_user_callback_key;
	extern GLFWcharfun          prev_user_callback_char;
	extern GLFWmonitorfun       prev_user_callback_monitor;

	static void* memAlloc(size_t a_size, void* a_userData)
	{
		BX_UNUSED(a_userData);
		return BX_ALLOC(imgui_context._allocator, a_size);
	}

	static void memFree(void* a_ptr, void* a_userData)
	{
		BX_UNUSED(a_userData);
		BX_FREE(imgui_context._allocator, a_ptr);
	}

	static const bgfx::EmbeddedShader s_embeddedShaders[] =
	{
		BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
		BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
		BGFX_EMBEDDED_SHADER(vs_imgui_image),
		BGFX_EMBEDDED_SHADER(fs_imgui_image),

		BGFX_EMBEDDED_SHADER_END()
	};

	bool ImGui_ImplBGFX_AddFonts();

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
