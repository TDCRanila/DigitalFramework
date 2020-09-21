/*
 * Copyright 2014-2015 Daniel Collin. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/allocator.h>
#include <bx/math.h>
#include <bx/timer.h>
#include <imgui/imgui.h>
#include <imgui/imgui_user.h>

#include <CoreSystems/WindowManagement.h>
#include <CoreSystems/InputManagement.h> 
#include <CoreSystems/ImGuiLayer.h>
#include <Utility/TemplateUtility.h>
#include <Defines/InputDefines.h>

 //#define USE_ENTRY 1

#ifndef USE_ENTRY
#	define USE_ENTRY 0
#endif // USE_ENTRY

#if USE_ENTRY
#	include "../entry/entry.h"
#	include "../entry/input.h"
#endif // USE_ENTRY

#include <imgui/resources/vs_ocornut_imgui.bin.h>
#include <imgui/resources/fs_ocornut_imgui.bin.h>
#include <imgui/resources/vs_imgui_image.bin.h>
#include <imgui/resources/fs_imgui_image.bin.h>

#include <imgui/resources/roboto_regular.ttf.h>
#include <imgui/resources/robotomono_regular.ttf.h>
#include <imgui/resources/icons_kenney.ttf.h>
#include <imgui/resources/icons_font_awesome.ttf.h>

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
	BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(vs_imgui_image),
	BGFX_EMBEDDED_SHADER(fs_imgui_image),

	BGFX_EMBEDDED_SHADER_END()
};

struct FontRangeMerge
{
	const void* data;
	size_t      size;
	ImWchar     ranges[3];
};

#define ICON_MIN_KI 0xe900
#define ICON_MAX_KI 0xe9e3

#define ICON_MIN_FA 0xf000
#define ICON_MAX_FA 0xf2e0

static FontRangeMerge s_fontRangeMerge[] =
{
	{ s_iconsKenneyTtf,      sizeof(s_iconsKenneyTtf),      { ICON_MIN_KI, ICON_MAX_KI, 0 } },
	{ s_iconsFontAwesomeTtf, sizeof(s_iconsFontAwesomeTtf), { ICON_MIN_FA, ICON_MAX_FA, 0 } },
};

static void* memAlloc(size_t _size, void* _userData);
static void memFree(void* _ptr, void* _userData);

/// Returns true if both internal transient index and vertex buffer have
/// enough space.
///
/// @param[in] _numVertices Number of vertices.
/// @param[in] _layout Vertex layout.
/// @param[in] _numIndices Number of indices.
///
inline bool checkAvailTransientBuffers(uint32_t _numVertices, const bgfx::VertexLayout& _layout, uint32_t _numIndices)
{
	return _numVertices == bgfx::getAvailTransientVertexBuffer(_numVertices, _layout)
		&& (0 == _numIndices || _numIndices == bgfx::getAvailTransientIndexBuffer(_numIndices))
		;
}

struct OcornutImguiContext
{
	void render(ImDrawData* _drawData)
	{
		const ImGuiIO& io = ImGui::GetIO();
		const float width = io.DisplaySize.x;
		const float height = io.DisplaySize.y;

		bgfx::setViewName(m_viewId, "ImGui");
		bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);

		const bgfx::Caps* caps = bgfx::getCaps();
		{
			float ortho[16];
			bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
			bgfx::setViewTransform(m_viewId, NULL, ortho);
			bgfx::setViewRect(m_viewId, 0, 0, uint16_t(width), uint16_t(height));
		}

		// Render command lists
		for (int32_t ii = 0, num = _drawData->CmdListsCount; ii < num; ++ii)
		{
			bgfx::TransientVertexBuffer tvb;
			bgfx::TransientIndexBuffer tib;

			const ImDrawList* drawList = _drawData->CmdLists[ii];
			uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
			uint32_t numIndices = (uint32_t)drawList->IdxBuffer.size();

			if (!checkAvailTransientBuffers(numVertices, m_layout, numIndices))
			{
				// not enough space in transient buffer just quit drawing the rest...
				break;
			}

			bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_layout);
			bgfx::allocTransientIndexBuffer(&tib, numIndices);

			ImDrawVert* verts = (ImDrawVert*)tvb.data;
			bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

			ImDrawIdx* indices = (ImDrawIdx*)tib.data;
			bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

			uint32_t offset = 0;
			for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
			{
				if (cmd->UserCallback)
				{
					cmd->UserCallback(drawList, cmd);
				}
				else if (0 != cmd->ElemCount)
				{
					uint64_t state = 0
						| BGFX_STATE_WRITE_RGB
						| BGFX_STATE_WRITE_A
						| BGFX_STATE_MSAA
						;

					bgfx::TextureHandle th = m_texture;
					bgfx::ProgramHandle program = m_program;

					if (NULL != cmd->TextureId)
					{
						union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; } texture = { cmd->TextureId };
						state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
							? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
							: BGFX_STATE_NONE
							;
						th = texture.s.handle;
						if (0 != texture.s.mip)
						{
							const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
							bgfx::setUniform(u_imageLodEnabled, lodEnabled);
							program = m_imageProgram;
						}
					}
					else
					{
						state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
					}

					const uint16_t xx = uint16_t(bx::max(cmd->ClipRect.x, 0.0f));
					const uint16_t yy = uint16_t(bx::max(cmd->ClipRect.y, 0.0f));
					bgfx::setScissor(xx, yy
						, uint16_t(bx::min(cmd->ClipRect.z, 65535.0f) - xx)
						, uint16_t(bx::min(cmd->ClipRect.w, 65535.0f) - yy)
					);

					bgfx::setState(state);
					bgfx::setTexture(0, s_tex, th);
					bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
					bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
					bgfx::submit(m_viewId, program);
				}

				offset += cmd->ElemCount;
			}
		}
	}

	void create(float _fontSize, bx::AllocatorI* _allocator)
	{
		m_allocator = _allocator;

		if (NULL == _allocator)
		{
			static bx::DefaultAllocator allocator;
			m_allocator = &allocator;
		}

		m_viewId = 255;
		m_lastScroll = 0;
		m_last = bx::getHPCounter();

		ImGui::SetAllocatorFunctions(memAlloc, memFree, NULL);

		m_imgui = ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(1280.0f, 720.0f);
		io.DeltaTime = 1.0f / 60.0f;
		io.IniFilename = NULL;

		setupStyle(true);

		io.KeyMap[ImGuiKey_Tab]			= to_underlying(DKey::TAB);
		io.KeyMap[ImGuiKey_LeftArrow]	= to_underlying(DKey::LEFT);
		io.KeyMap[ImGuiKey_RightArrow]	= to_underlying(DKey::RIGHT);
		io.KeyMap[ImGuiKey_UpArrow]		= to_underlying(DKey::UP);
		io.KeyMap[ImGuiKey_DownArrow]	= to_underlying(DKey::DOWN);
		io.KeyMap[ImGuiKey_PageUp]		= to_underlying(DKey::PAGE_UP);
		io.KeyMap[ImGuiKey_PageDown]	= to_underlying(DKey::PAGE_DOWN);
		io.KeyMap[ImGuiKey_Home]		= to_underlying(DKey::HOME);
		io.KeyMap[ImGuiKey_End]			= to_underlying(DKey::END);
		io.KeyMap[ImGuiKey_Insert]		= to_underlying(DKey::INSERT);
		io.KeyMap[ImGuiKey_Delete]		= to_underlying(DKey::KDELETE);
		io.KeyMap[ImGuiKey_Backspace]	= to_underlying(DKey::BACKSPACE);
		io.KeyMap[ImGuiKey_Space]		= to_underlying(DKey::SPACE);
		io.KeyMap[ImGuiKey_Enter]		= to_underlying(DKey::ENTER);
		io.KeyMap[ImGuiKey_Escape]		= to_underlying(DKey::ESCAPE);
		io.KeyMap[ImGuiKey_A]			= to_underlying(DKey::A);
		io.KeyMap[ImGuiKey_C]			= to_underlying(DKey::C);
		io.KeyMap[ImGuiKey_V]			= to_underlying(DKey::V);
		io.KeyMap[ImGuiKey_X]			= to_underlying(DKey::X);
		io.KeyMap[ImGuiKey_Y]			= to_underlying(DKey::Y);
		io.KeyMap[ImGuiKey_Z]			= to_underlying(DKey::Z);

		io.ConfigFlags |= 0 | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

		// TODO Implement Clipboard 
		// io.SetClipboardTextFn = ImGui_ImplGlfwGL3_SetClipboardText;
		// io.GetClipboardTextFn = ImGui_ImplGlfwGL3_GetClipboardText;

		//io.NavInputs[ImGuiNavInput_Activate] = (int)entry::Key::GamepadA;
		//io.NavInputs[ImGuiNavInput_Cancel] = (int)entry::Key::GamepadB;
		//		io.NavInputs[ImGuiNavInput_Input]       = (int)entry::Key::;
		//		io.NavInputs[ImGuiNavInput_Menu]        = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_DpadLeft] = (int)entry::Key::GamepadLeft;
		//io.NavInputs[ImGuiNavInput_DpadRight] = (int)entry::Key::GamepadRight;
		//io.NavInputs[ImGuiNavInput_DpadUp] = (int)entry::Key::GamepadUp;
		//io.NavInputs[ImGuiNavInput_DpadDown] = (int)entry::Key::GamepadDown;
		//		io.NavInputs[ImGuiNavInput_LStickLeft]  = (int)entry::Key::;
		//		io.NavInputs[ImGuiNavInput_LStickRight] = (int)entry::Key::;
		//		io.NavInputs[ImGuiNavInput_LStickUp]    = (int)entry::Key::;
		//		io.NavInputs[ImGuiNavInput_LStickDown]  = (int)entry::Key::;
		//		io.NavInputs[ImGuiNavInput_FocusPrev]   = (int)entry::Key::;
		//		io.NavInputs[ImGuiNavInput_FocusNext]   = (int)entry::Key::;
		//		io.NavInputs[ImGuiNavInput_TweakSlow]   = (int)entry::Key::;
		//		io.NavInputs[ImGuiNavInput_TweakFast]   = (int)entry::Key::;

		bgfx::RendererType::Enum type = bgfx::getRendererType();
		m_program = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui")
			, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui")
			, true
		);

		u_imageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
		m_imageProgram = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image")
			, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image")
			, true
		);

		m_layout
			.begin()
			.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();

		s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

		uint8_t* data;
		int32_t width;
		int32_t height;
		{
			ImFontConfig config;
			config.FontDataOwnedByAtlas = false;
			config.MergeMode = false;
			//			config.MergeGlyphCenterV = true;

			const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
			m_font[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTtf, sizeof(s_robotoRegularTtf), _fontSize, &config, ranges);
			m_font[ImGui::Font::Mono] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), _fontSize - 3.0f, &config, ranges);

			config.MergeMode = true;
			config.DstFont = m_font[ImGui::Font::Regular];

			for (uint32_t ii = 0; ii < BX_COUNTOF(s_fontRangeMerge); ++ii)
			{
				const FontRangeMerge& frm = s_fontRangeMerge[ii];

				io.Fonts->AddFontFromMemoryTTF((void*)frm.data
					, (int)frm.size
					, _fontSize - 3.0f
					, &config
					, frm.ranges
				);
			}
		}

		io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

		m_texture = bgfx::createTexture2D(
			(uint16_t)width
			, (uint16_t)height
			, false
			, 1
			, bgfx::TextureFormat::BGRA8
			, 0
			, bgfx::copy(data, width * height * 4)
		);
		
		// TODO ImGui Dock
		//ImGui::InitDockContext();
	}

	void destroy()
	{
		// TODO ImGui Dock
		//ImGui::ShutdownDockContext();
		ImGui::DestroyContext(m_imgui);

		bgfx::destroy(s_tex);
		bgfx::destroy(m_texture);

		bgfx::destroy(u_imageLodEnabled);
		bgfx::destroy(m_imageProgram);
		bgfx::destroy(m_program);

		m_allocator = NULL;
	}

	void setupStyle(bool _dark)
	{
		// TODO: add Invert/Light Version
		UNUSED(_dark);
		// Style: https://github.com/ocornut/imgui/issues/707#issuecomment-670976818
		constexpr auto ColourFromBytes = [](uint8 r, uint8 g, uint8 b)
		{
			return ImVec4(static_cast<float32>(r) / 255.0f, static_cast<float32>(g) / 255.0f, static_cast<float32>(b) / 255.0f, 1.0f);
		};

		ImGuiStyle& style		= ImGui::GetStyle();
		ImVec4* colours			= style.Colors;

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
		colours[ImGuiCol_ModalWindowDarkening]	= bgColor;
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
	}

	void beginFrame(const DCore::InputData& a_input_data, const DCore::WindowDimension& a_window_dimension, bgfx::ViewId _viewId)
	{
		m_viewId = _viewId;

		ImGuiIO& io = ImGui::GetIO();
		for (const uint32& c : a_input_data._buffered_characters)
		{
			if (c >= 0)
			{
				io.AddInputCharacter(c);
			}
		}

		io.DisplaySize = ImVec2((float)a_window_dimension._current_width, (float)a_window_dimension._current_height);
		io.DisplayFramebufferScale = ImVec2(a_window_dimension._current_width > 0 ? ((float)a_window_dimension._current_frame_width / a_window_dimension._current_width) : 0, a_window_dimension._current_height > 0 ? ((float)a_window_dimension._current_frame_height / a_window_dimension._current_height) : 0);

		const int64_t now = bx::getHPCounter();
		const int64_t frameTime = now - m_last;
		m_last = now;
		const double freq = double(bx::getHPFrequency());
		io.DeltaTime = float(frameTime / freq);

		io.MousePos = ImVec2(a_input_data._cursor_position.x, a_input_data._cursor_position.y);

		for (auto const& [key, action] : a_input_data._buffered_keys)
		{
			int32 pressed_key	= to_underlying(key);
			int32 max_mouse_key = to_underlying(DMouse::BUTTON_6);
			if (action == DKeyAction::PRESSED)
			{
				io.KeysDown[pressed_key] = true;
				if (pressed_key <= max_mouse_key)
					io.MouseDown[pressed_key] = true;
			}
			if (action == DKeyAction::RELEASED)
			{
				io.KeysDown[pressed_key] = false;
				if (pressed_key <= max_mouse_key)
					io.MouseDown[pressed_key] = false;
			}
		}

		io.MouseWheel	= static_cast<float>(a_input_data._scroll_offset.y - m_lastScroll);
		m_lastScroll	= static_cast<int32_t>(a_input_data._scroll_offset.y);

		io.KeyCtrl		= io.KeysDown[to_underlying(DKey::LEFT_CONTROL)]	|| io.KeysDown[to_underlying(DKey::RIGHT_CONTROL)];
		io.KeyShift		= io.KeysDown[to_underlying(DKey::LEFT_SHIFT)]		|| io.KeysDown[to_underlying(DKey::RIGHT_SHIFT)];
		io.KeyAlt		= io.KeysDown[to_underlying(DKey::LEFT_ALT)]		|| io.KeysDown[to_underlying(DKey::RIGHT_ALT)];
		io.KeySuper		= io.KeysDown[to_underlying(DKey::LEFT_SUPER)]		|| io.KeysDown[to_underlying(DKey::RIGHT_SUPER)];

		ImGui::NewFrame();

		// TODO: ImGuizmo
		//ImGuizmo::BeginFrame();
	}

	void endFrame()
	{
		ImGui::Render();
		render(ImGui::GetDrawData());
	}

	ImGuiContext* m_imgui;
	bx::AllocatorI* m_allocator;
	bgfx::VertexLayout  m_layout;
	bgfx::ProgramHandle m_program;
	bgfx::ProgramHandle m_imageProgram;
	bgfx::TextureHandle m_texture;
	bgfx::UniformHandle s_tex;
	bgfx::UniformHandle u_imageLodEnabled;
	ImFont* m_font[ImGui::Font::Count];
	int64_t m_last;
	int32_t m_lastScroll;
	bgfx::ViewId m_viewId;
};

static OcornutImguiContext s_ctx;

static void* memAlloc(size_t _size, void* _userData)
{
	BX_UNUSED(_userData);
	return BX_ALLOC(s_ctx.m_allocator, _size);
}

static void memFree(void* _ptr, void* _userData)
{
	BX_UNUSED(_userData);
	BX_FREE(s_ctx.m_allocator, _ptr);
}

void imguiCreate(float _fontSize, bx::AllocatorI* _allocator)
{
	s_ctx.create(_fontSize, _allocator);
}

void imguiDestroy()
{
	s_ctx.destroy();
}

void imguiBeginFrame(const DCore::InputData& a_input_data, const DCore::WindowDimension& a_window_dimension, bgfx::ViewId _viewId)
{
	s_ctx.beginFrame(a_input_data, a_window_dimension, _viewId);
}

void imguiEndFrame()
{
	s_ctx.endFrame();
}

namespace ImGui
{
	void PushFont(Font::Enum _font)
	{
		PushFont(s_ctx.m_font[_font]);
	}
} // namespace ImGui

BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4505); // error C4505: '' : unreferenced local function has been removed
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wunused-function"); // warning: 'int rect_width_compare(const void*, const void*)' defined but not used
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wunknown-pragmas")
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wtype-limits"); // warning: comparison is always true due to limited range of data type
#define STBTT_malloc(_size, _userData) memAlloc(_size, _userData)
#define STBTT_free(_ptr, _userData) memFree(_ptr, _userData)
#define STB_RECT_PACK_IMPLEMENTATION
//#include <stb/stb_rect_pack.h>
#include <imgui/imstb_rectpack.h>
#define STB_TRUETYPE_IMPLEMENTATION
//#include <stb/stb_truetype.h>
#include <imgui/imstb_truetype.h>
BX_PRAGMA_DIAGNOSTIC_POP();
