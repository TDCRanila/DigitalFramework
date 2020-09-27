#include <CoreSystems/ImGuiLayer.h>

#include <bgfx/embedded_shader.h>
#include <bx/allocator.h>
#include <bx/math.h>
#include <bx/timer.h>

#include <CoreSystems/WindowManagement.h>
#include <Utility/TemplateUtility.h>
#include <Defines/InputDefines.h>

#include <imgui/resources/vs_ocornut_imgui.bin.h>
#include <imgui/resources/fs_ocornut_imgui.bin.h>
#include <imgui/resources/vs_imgui_image.bin.h>
#include <imgui/resources/fs_imgui_image.bin.h>
 
#include <imgui/resources/roboto_regular.ttf.h>
#include <imgui/resources/robotomono_regular.ttf.h>
#include <imgui/resources/icons_kenney.ttf.h>
#include <imgui/resources/icons_font_awesome.ttf.h>

#define ICON_MIN_KI 0xe900
#define ICON_MAX_KI 0xe9e3

#define ICON_MIN_FA 0xf000
#define ICON_MAX_FA 0xf2e0

namespace detail
{
	static void* memAlloc(size_t a_size, void* a_userData)
	{
		BX_UNUSED(a_userData);
		return BX_ALLOC(DCore::ImGuiLayer::_imgui_context._allocator, a_size);
	}

	static void memFree(void* a_ptr, void* a_userData)
	{
		BX_UNUSED(a_userData);
		BX_FREE(DCore::ImGuiLayer::_imgui_context._allocator, a_ptr);
	}

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

	static FontRangeMerge s_fontRangeMerge[] =
	{
		{ s_iconsKenneyTtf,      sizeof(s_iconsKenneyTtf),      { ICON_MIN_KI, ICON_MAX_KI, 0 } },
		{ s_iconsFontAwesomeTtf, sizeof(s_iconsFontAwesomeTtf), { ICON_MIN_FA, ICON_MAX_FA, 0 } },
	};
}

namespace DCore
{
	OcornutImguiContext ImGuiLayer::_imgui_context;

	void ImGuiLayer::CreateContext(float32 a_fontSize, bx::AllocatorI* a_allocator)
	{
		_imgui_context.Create(a_fontSize, a_allocator);
	}

	void ImGuiLayer::DestroyContext()
	{
		_imgui_context.Destroy();
	}

	void ImGuiLayer::BeginFrame(const DCore::InputData& a_input_data, const DCore::WindowDimension& a_window_dimension, bgfx::ViewId a_view_id)
	{
		_imgui_context.BeginFrame(a_input_data, a_window_dimension, a_view_id);
	}

	void ImGuiLayer::EndFrame()
	{
		_imgui_context.EndFrame();
	}

	void OcornutImguiContext::Create(float32 a_font_size, bx::AllocatorI* a_allocator)
	{
		_allocator = a_allocator;

		if (NULL == _allocator)
		{
			static bx::DefaultAllocator allocator;
			_allocator = &allocator;
		}

		ImGui::SetAllocatorFunctions(detail::memAlloc, detail::memFree, NULL);

		_view_id		= 255;
		_last_scroll	= 0;
		_last			= bx::getHPCounter();

		_imgui_context	= ImGui::CreateContext();

		SetupStyle();

		ImGuiIO& io		= ImGui::GetIO();
		io.DisplaySize	= ImVec2(1280.0f, 720.0f);
		io.DeltaTime	= 1.0f / 60.0f;
		io.IniFilename	= NULL;

		// Clipbord Callback Hooks
		io.SetClipboardTextFn = WindowManagementSystem::GLFWWindowCallBacks::glfw_set_clipboard_string;
		io.GetClipboardTextFn = WindowManagementSystem::GLFWWindowCallBacks::glfw_get_clipboard_string;

		// Key Mappings
		io.ConfigFlags |= 0 | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

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

		// TODO
		// Navigation Input Key Mappings
		//io.NavInputs[ImGuiNavInput_Activate] = (int)entry::Key::GamepadA;
		//io.NavInputs[ImGuiNavInput_Cancel] = (int)entry::Key::GamepadB;
		//io.NavInputs[ImGuiNavInput_Input] = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_Menu] = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_DpadLeft] = (int)entry::Key::GamepadLeft;
		//io.NavInputs[ImGuiNavInput_DpadRight] = (int)entry::Key::GamepadRight;
		//io.NavInputs[ImGuiNavInput_DpadUp] = (int)entry::Key::GamepadUp;
		//io.NavInputs[ImGuiNavInput_DpadDown] = (int)entry::Key::GamepadDown;
		//io.NavInputs[ImGuiNavInput_LStickLeft]  = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_LStickRight] = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_LStickUp] = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_LStickDown] = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_FocusPrev] = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_FocusNext] = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_TweakSlow] = (int)entry::Key::;
		//io.NavInputs[ImGuiNavInput_TweakFast]= (int)entry::Key::;

		bgfx::RendererType::Enum render_type = bgfx::getRendererType();

		bool destroy_shaders = true;

		_program = bgfx::createProgram(
			bgfx::createEmbeddedShader(detail::s_embeddedShaders, render_type, "vs_ocornut_imgui")
			, bgfx::createEmbeddedShader(detail::s_embeddedShaders, render_type, "fs_ocornut_imgui")
			, destroy_shaders
		);

		u_imageLodEnabled = bgfx::createUniform("u_imageLodEnabled"	, bgfx::UniformType::Vec4);

		_image_program = bgfx::createProgram(
			  bgfx::createEmbeddedShader(detail::s_embeddedShaders, render_type, "vs_imgui_image")
			, bgfx::createEmbeddedShader(detail::s_embeddedShaders, render_type, "fs_imgui_image")
			, destroy_shaders);

		s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);
		_layout.begin()
			.add(bgfx::Attrib::Position	, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0	, 4, bgfx::AttribType::Uint8, true)
			.end();

		uint8* data;
		int32 width;
		int32 height;
		{
			ImFontConfig config;
			config.FontDataOwnedByAtlas = false;
			config.MergeMode			= false;

			const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
			_font[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTtf		, sizeof(s_robotoRegularTtf)	, a_font_size		, &config, ranges);
			_font[ImGui::Font::Mono]	= io.Fonts->AddFontFromMemoryTTF((void*)s_robotoMonoRegularTtf	, sizeof(s_robotoMonoRegularTtf), a_font_size - 3.0f, &config, ranges);

			config.MergeMode = true;
			config.DstFont	= _font[ImGui::Font::Regular];

			for (uint32 ii = 0; ii < BX_COUNTOF(detail::s_fontRangeMerge); ++ii)
			{
				const detail::FontRangeMerge& frm = detail::s_fontRangeMerge[ii];
				io.Fonts->AddFontFromMemoryTTF((void*)frm.data, static_cast<int>(frm.size), a_font_size - 3.0f, &config, frm.ranges);
			}
		}

		io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

		_texture_handle = bgfx::createTexture2D(
			  static_cast<uint16>(width)
			, static_cast<uint16>(height)
			, false
			, 1
			, bgfx::TextureFormat::BGRA8
			, 0
			, bgfx::copy(data, width * height * 4)
		);

		// TODO ImGui Dock
		//ImGui::InitDockContext();
	}

	void OcornutImguiContext::Destroy()
	{
		// TODO ImGui Dock
		//ImGui::ShutdownDockContext();
		ImGui::DestroyContext(_imgui_context);

		bgfx::destroy(s_tex);
		bgfx::destroy(_texture_handle);

		bgfx::destroy(u_imageLodEnabled);
		bgfx::destroy(_image_program);
		bgfx::destroy(_program);

		_allocator = NULL;
	}
	
	void OcornutImguiContext::Render(ImDrawData* a_draw_data)
	{
		const ImGuiIO& io	= ImGui::GetIO();
		const float width	= io.DisplaySize.x;
		const float height	= io.DisplaySize.y;

		bgfx::setViewName(_view_id, "ImGui");
		bgfx::setViewMode(_view_id, bgfx::ViewMode::Sequential);

		const bgfx::Caps* caps = bgfx::getCaps();
		{
			float ortho[16];
			bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
			bgfx::setViewTransform(_view_id, NULL, ortho);
			bgfx::setViewRect(_view_id, 0, 0, uint16_t(width), uint16_t(height));
		}

		// Render command lists
		for (int32_t ii = 0, num = a_draw_data->CmdListsCount; ii < num; ++ii)
		{
			bgfx::TransientVertexBuffer tvb;
			bgfx::TransientIndexBuffer tib;

			const ImDrawList* drawList = a_draw_data->CmdLists[ii];
			uint32 numVertices	= (uint32)drawList->VtxBuffer.size();
			uint32 numIndices	= (uint32)drawList->IdxBuffer.size();

			bool result = numVertices == bgfx::getAvailTransientVertexBuffer(numVertices, _layout) && (0 == numIndices || numIndices == bgfx::getAvailTransientIndexBuffer(numIndices));
			if (!result)
			{
				// not enough space in transient buffer just quit drawing the rest...
				break;
			}

			bgfx::allocTransientVertexBuffer(&tvb, numVertices, _layout);
			bgfx::allocTransientIndexBuffer(&tib, numIndices);

			ImDrawVert* verts = (ImDrawVert*)tvb.data;
			bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

			ImDrawIdx* indices = (ImDrawIdx*)tib.data;
			bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

			uint32 offset = 0;
			for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
			{
				if (cmd->UserCallback)
				{
					cmd->UserCallback(drawList, cmd);
				}
				else if (0 != cmd->ElemCount)
				{
					uint64 state = 0
						| BGFX_STATE_WRITE_RGB
						| BGFX_STATE_WRITE_A
						| BGFX_STATE_MSAA
						;

					bgfx::TextureHandle th = _texture_handle;
					bgfx::ProgramHandle program = _program;

					if (NULL != cmd->TextureId)
					{
						union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8 flags; uint8 mip; } s; } texture = { cmd->TextureId };
						state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
							? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
							: BGFX_STATE_NONE
							;
						th = texture.s.handle;
						if (0 != texture.s.mip)
						{
							const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
							bgfx::setUniform(u_imageLodEnabled, lodEnabled);
							program = _image_program;
						}
					}
					else
					{
						state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
					}

					const uint16 xx = uint16(bx::max(cmd->ClipRect.x, 0.0f));
					const uint16 yy = uint16(bx::max(cmd->ClipRect.y, 0.0f));
					bgfx::setScissor(xx, yy
						, uint16(bx::min(cmd->ClipRect.z, static_cast<float>(DLimits::uint16_max)) - xx)
						, uint16(bx::min(cmd->ClipRect.w, static_cast<float>(DLimits::uint16_max)) - yy)
					);

					bgfx::setState(state);
					bgfx::setTexture(0, s_tex, th);
					bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
					bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
					bgfx::submit(_view_id, program);
				}

				offset += cmd->ElemCount;
			}
		}
	}

	void OcornutImguiContext::BeginFrame(const DCore::InputData& a_input_data, const DCore::WindowDimension& a_window_dimension, bgfx::ViewId a_view_id)
	{
		_view_id = a_view_id;

		// Character Input
		ImGuiIO& io = ImGui::GetIO();
		for (const uint32& c : a_input_data._buffered_characters)
		{
			if (c >= 0)
			{
				io.AddInputCharacter(c);
			}
		}

		// Window Size Adjustment
		const float32 f_height	= static_cast<float32>(a_window_dimension._current_frame_height);
		const float32 f_width	= static_cast<float32>(a_window_dimension._current_frame_width);
		const float32 height	= static_cast<float32>(a_window_dimension._current_height);
		const float32 width		= static_cast<float32>(a_window_dimension._current_width);
		io.DisplaySize				= ImVec2(width, height);
		io.DisplayFramebufferScale	= ImVec2(width > 0 ? (f_width / width) : 0.f, height > 0.f ? (f_height / height) : 0);

		// Frame Time
		const int64 now		= bx::getHPCounter();
		const int64 frameTime = now - _last;
		_last = now;
		const float64 freq	= static_cast<float64>(bx::getHPFrequency());
		io.DeltaTime		= static_cast<float32>(frameTime / freq);

		// Reading Input
		io.MousePos = ImVec2(a_input_data._cursor_position.x, a_input_data._cursor_position.y);

		for (auto const& [key, action] : a_input_data._buffered_keys)
		{
			int32 pressed_key	= key;
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

		io.MouseWheel	= static_cast<float>(a_input_data._scroll_offset.y - _last_scroll);
		_last_scroll	= static_cast<int32>(a_input_data._scroll_offset.y);

		io.KeyCtrl	= io.KeysDown[to_underlying(DKey::LEFT_CONTROL)]	|| io.KeysDown[to_underlying(DKey::RIGHT_CONTROL)];
		io.KeyShift = io.KeysDown[to_underlying(DKey::LEFT_SHIFT)]		|| io.KeysDown[to_underlying(DKey::RIGHT_SHIFT)];
		io.KeyAlt	= io.KeysDown[to_underlying(DKey::LEFT_ALT)]		|| io.KeysDown[to_underlying(DKey::RIGHT_ALT)];
		io.KeySuper = io.KeysDown[to_underlying(DKey::LEFT_SUPER)]		|| io.KeysDown[to_underlying(DKey::RIGHT_SUPER)];

		ImGui::NewFrame();

		// TODO: ImGuizmo
		//ImGuizmo::BeginFrame();
	}

	void OcornutImguiContext::EndFrame()
	{
		ImGui::Render();
		Render(ImGui::GetDrawData());
	}

	void OcornutImguiContext::SetupStyle()
	{
		// TODO: add Invert/Light Version

		// Style: https://github.com/ocornut/imgui/issues/707#issuecomment-670976818
		constexpr auto ColourFromBytes = [](uint8 r, uint8 g, uint8 b)
		{
			return ImVec4(static_cast<float32>(r) / 255.0f, static_cast<float32>(g) / 255.0f, static_cast<float32>(b) / 255.0f, 1.0f);
		};

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colours = style.Colors;

		const ImVec4 bgColor = ColourFromBytes(30, 30, 31);
		const ImVec4 lightBgColor = ColourFromBytes(72, 72, 75);
		const ImVec4 veryLightBgColor = ColourFromBytes(80, 80, 85);

		const ImVec4 panelColor = ColourFromBytes(41, 41, 45);
		const ImVec4 panelHoverColor = ColourFromBytes(245, 80, 0);
		const ImVec4 panelActiveColor = ColourFromBytes(190, 55, 10);

		const ImVec4 textColor = ColourFromBytes(255, 255, 255);
		const ImVec4 textDisabledColor = ColourFromBytes(151, 151, 151);
		const ImVec4 borderColor = ColourFromBytes(78, 78, 78);

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

} // End of namespace ~ DCore.

namespace ImGui
{
	ImTextureID ToID(bgfx::TextureHandle a_handle, uint8 a_flags, uint8 a_mip)
	{
		union
		{
			struct
			{
				bgfx::TextureHandle handle;
				uint8 flags;
				uint8 mip;
			}
			s; ImTextureID id;
		} tex;

		tex.s.handle	= a_handle;
		tex.s.flags		= a_flags;
		tex.s.mip		= a_mip;
		return tex.id;
	}

	// Helper function for passing bgfx::TextureHandle to ImGui::Image.
	void Image(bgfx::TextureHandle a_handle, uint8 a_flags, uint8 a_mip, const ImVec2& a_size, const ImVec2& a_uv0, const ImVec2& a_uv1
		, const ImVec4& a_tint_colour, const ImVec4& a_border_colour)
	{
		Image(ToID(a_handle, a_flags, a_mip), a_size, a_uv0, a_uv1, a_tint_colour, a_border_colour);
	}

	// Helper function for passing bgfx::TextureHandle to ImGui::Image.
	void Image(bgfx::TextureHandle a_handle, const ImVec2& a_size, const ImVec2& a_uv0, const ImVec2& a_uv1
		, const ImVec4& a_tint_colour, const ImVec4& a_border_colour)
	{
		Image(a_handle, IMGUI_FLAGS_ALPHA_BLEND, 0, a_size, a_uv0, a_uv1, a_tint_colour, a_border_colour);
	}

	// Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
	bool ImageButton(bgfx::TextureHandle a_handle, uint8 a_flags, uint8 a_mip, const ImVec2& a_size
		, const ImVec2& a_uv0, const ImVec2& a_uv1, int32 a_frame_padding
		, const ImVec4& a_backgroundg_colour, const ImVec4& a_tint_colour)
	{
		return ImageButton(ToID(a_handle, a_flags, a_mip), a_size, a_uv0, a_uv1, a_frame_padding, a_backgroundg_colour, a_tint_colour);
	}

	// Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
	bool ImageButton(bgfx::TextureHandle a_handle, const ImVec2& a_size, const ImVec2& a_uv0, const ImVec2& a_uv1, int32 a_frame_padding
		, const ImVec4& a_backgroundg_colour, const ImVec4& a_tint_colour)
	{
		return ImageButton(a_handle, IMGUI_FLAGS_ALPHA_BLEND, 0, a_size, a_uv0, a_uv1, a_frame_padding, a_backgroundg_colour, a_tint_colour);
	}

	void PushFont(Font::Enum _font)
	{
		PushFont(DCore::ImGuiLayer::_imgui_context._font[_font]);
	}

} // End of namespace ~ ImGui.

BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4505); // error C4505: '' : unreferenced local function has been removed
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wunused-function"); // warning: 'int rect_width_compare(const void*, const void*)' defined but not used
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wunknown-pragmas")
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wtype-limits"); // warning: comparison is always true due to limited range of data type
#define STBTT_malloc(_size, _userData) detail::memAlloc(_size, _userData)
#define STBTT_free(_ptr, _userData) detail::memFree(_ptr, _userData)
#define STB_RECT_PACK_IMPLEMENTATION
//#include <stb/stb_rect_pack.h>
#include <imgui/imstb_rectpack.h>
#define STB_TRUETYPE_IMPLEMENTATION
//#include <stb/stb_truetype.h>
#include <imgui/imstb_truetype.h>
BX_PRAGMA_DIAGNOSTIC_POP();
