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

#include <CoreSystems/InputManagement.h> 
#include <CoreSystems/ImGuiLayer.h>
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

		io.KeyMap[ImGuiKey_Tab]			= KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow]	= KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow]	= KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow]		= KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow]	= KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp]		= KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown]	= KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home]		= KEY_HOME;
		io.KeyMap[ImGuiKey_End]			= KEY_END;
		io.KeyMap[ImGuiKey_Insert]		= KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete]		= KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace]	= KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space]		= KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter]		= KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape]		= KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A]			= KEY_A;
		io.KeyMap[ImGuiKey_C]			= KEY_C;
		io.KeyMap[ImGuiKey_V]			= KEY_V;
		io.KeyMap[ImGuiKey_X]			= KEY_X;
		io.KeyMap[ImGuiKey_Y]			= KEY_Y;
		io.KeyMap[ImGuiKey_Z]			= KEY_Z;

		io.ConfigFlags |= 0 | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

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
		// Doug Binks' darl color scheme
		// https://gist.github.com/dougbinks/8089b4bbaccaaf6fa204236978d165a9
		ImGuiStyle& style = ImGui::GetStyle();
		if (_dark)
		{
			ImGui::StyleColorsDark(&style);
		}
		else
		{
			ImGui::StyleColorsLight(&style);
		}

		style.FrameRounding = 4.0f;
		style.WindowBorderSize = 0.0f;
	}

	void beginFrame(const DCore::InputData& a_input_data, int _width, int _height, bgfx::ViewId _viewId)
	{
		m_viewId = _viewId;

		ImGuiIO& io = ImGui::GetIO();
		for (const uint32& c : a_input_data._frame_buffered_characters)
		{
			if (c >= 0)
			{
				io.AddInputCharacter(c);
			}
		}

		io.DisplaySize = ImVec2((float)_width, (float)_height);

		const int64_t now = bx::getHPCounter();
		const int64_t frameTime = now - m_last;
		m_last = now;
		const double freq = double(bx::getHPFrequency());
		io.DeltaTime = float(frameTime / freq);

		io.MousePos = ImVec2(a_input_data._cursor_position.x, a_input_data._cursor_position.y);

		for (auto const& [key, action] : a_input_data._buffered_keys)
		{
			if (action == KEY_PRESS)
				io.MouseDown[key] = true;
			if (action == KEY_RELEASE)
				io.MouseDown[key] = false;
		}

		for (auto const& [key, action] : a_input_data._buffered_keys)
		{
			if (action == KEY_PRESS)
				io.KeysDown[key] = true;
			if (action == KEY_RELEASE)
				io.KeysDown[key] = false;
		}

		io.MouseWheel	= static_cast<float>(a_input_data._scroll_offset.y - m_lastScroll);
		m_lastScroll	= a_input_data._scroll_offset.y;

		io.KeyCtrl		= io.KeysDown[KEY_LEFT_CONTROL]	|| io.KeysDown[KEY_RIGHT_CONTROL];
		io.KeyShift		= io.KeysDown[KEY_LEFT_SHIFT]	|| io.KeysDown[KEY_RIGHT_SHIFT];
		io.KeyAlt		= io.KeysDown[KEY_LEFT_ALT]		|| io.KeysDown[KEY_RIGHT_ALT];
		io.KeySuper		= io.KeysDown[KEY_LEFT_SUPER]	|| io.KeysDown[KEY_RIGHT_SUPER];

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

void imguiBeginFrame(const DCore::InputData& a_input_data, uint16_t _width, uint16_t _height, bgfx::ViewId _viewId)
{
	s_ctx.beginFrame(a_input_data, _width, _height, _viewId);
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
