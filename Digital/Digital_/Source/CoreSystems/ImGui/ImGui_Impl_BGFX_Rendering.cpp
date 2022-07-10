#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Rendering.h>

#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Utility.h>

#include <bgfx/embedded_shader.h>
#include <bx/math.h>

#include <imgui/imgui.h>
#include <glfw/glfw3.h>

namespace DFW
{
	namespace DImGui
	{
		bool ImGui_ImplBGFX_InitGraphics()
		{
			const char* render_name = bgfx::getRendererName(bgfx::getRendererType());
			std::string const backend_renderer_name = std::string("bgfx-") + render_name;

			ImGuiIO& io = ImGui::GetIO();
			io.BackendRendererName = backend_renderer_name.c_str();

			// Setup Viewports interface if the bitflag is enabled.
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
			}

			return true;
		}

		void ImGui_ImplBGFX_ShutdownGraphics()
		{
			ImGui_ImplBGFX_DestroyDeviceObjects();
		}

		void ImGui_ImplBGFX_NewFrameGraphics()
		{
			if (!DImGui::imgui_rendering_context._are_graphic_devices_initialized)
				ImGui_ImplBGFX_CreateDeviceObjects();
		}

		void ImGui_ImplBGFX_RenderDrawData(ImDrawData const* const a_draw_data, bgfx::ViewId const a_view_id)
		{
			// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
			int32 const fb_width = static_cast<int32>(a_draw_data->DisplaySize.x * a_draw_data->FramebufferScale.x);
			int32 const fb_height = static_cast<int32>(a_draw_data->DisplaySize.y * a_draw_data->FramebufferScale.y);
			bool const skip_render = fb_width <= 0 || fb_height <= 0;
			if (skip_render)
				return;

			bgfx::setViewName(a_view_id, "ImGui");
			bgfx::setViewMode(a_view_id, bgfx::ViewMode::Sequential);

			bgfx::Caps const* caps = bgfx::getCaps();
			{
				float32 ortho[16];
				float32 const x = a_draw_data->DisplayPos.x;
				float32 const y = a_draw_data->DisplayPos.y;
				float32 const width = a_draw_data->DisplaySize.x;
				float32 const height = a_draw_data->DisplaySize.y;

				float32 z_near = 0.0f;
				float32 z_far = 1000.f;
				bx::mtxOrtho(ortho, x, x + width, y + height, y, z_near, z_far, 0.0f, caps->homogeneousDepth);
				bgfx::setViewTransform(a_view_id, NULL, ortho);
				bgfx::setViewRect(a_view_id, 0, 0, static_cast<uint16>(width), static_cast<uint16>(height));
			}

			ImVec2 const clipPos = a_draw_data->DisplayPos;       // (0,0) unless using multi-viewports
			ImVec2 const clipScale = a_draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

			// Render command lists
			for (int32 index = 0, num = a_draw_data->CmdListsCount; index < num; ++index)
			{
				bgfx::TransientVertexBuffer tvb;
				bgfx::TransientIndexBuffer tib;

				ImDrawList const* drawList = a_draw_data->CmdLists[index];
				uint32_t numVertices = static_cast<uint32>(drawList->VtxBuffer.size());
				uint32_t numIndices = static_cast<uint32>(drawList->IdxBuffer.size());

				auto checkAvailTransientBuffers = [](uint32 const _numVertices, bgfx::VertexLayout const& _layout, uint32 const _numIndices)
				{
					return _numVertices == bgfx::getAvailTransientVertexBuffer(_numVertices, _layout)
						&& (0 == _numIndices || _numIndices == bgfx::getAvailTransientIndexBuffer(_numIndices))
						;
				};

				bgfx::VertexLayout const& vertex_layout = imgui_rendering_context._vertex_layout;
				if (!checkAvailTransientBuffers(numVertices, vertex_layout, numIndices))
				{
					// not enough space in transient buffer just quit drawing the rest...
					break;
				}

				bgfx::allocTransientVertexBuffer(&tvb, numVertices, vertex_layout);
				bgfx::allocTransientIndexBuffer(&tib, numIndices, sizeof(ImDrawIdx) == 4);

				ImDrawVert* verts = reinterpret_cast<ImDrawVert*>(tvb.data);
				bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

				ImDrawIdx* indices = reinterpret_cast<ImDrawIdx*>(tib.data);
				bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

				bgfx::Encoder* encoder = bgfx::begin();

				uint32 offset = 0;
				for (ImDrawCmd const* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
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

						bgfx::TextureHandle texture_handle = DImGui::imgui_rendering_context._texture_handle;
						bgfx::ProgramHandle shader_program = DImGui::imgui_rendering_context._shader_program_handle;
						bgfx::ProgramHandle const image_program = DImGui::imgui_rendering_context._image_program_handle;
						bgfx::UniformHandle const image_lod = DImGui::imgui_rendering_context._image_lod_uniform_handle;
						bgfx::UniformHandle const texture_uniform = DImGui::imgui_rendering_context._texture_uniform_handle;

						if (NULL != cmd->TextureId)
						{
							union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8 flags; uint8 mip; } s; } texture = { cmd->TextureId };
							state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
								? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
								: BGFX_STATE_NONE
								;
							texture_handle = texture.s.handle;
							if (0 != texture.s.mip)
							{
								const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
								bgfx::setUniform(image_lod, lodEnabled);
								shader_program = image_program;
							}
						}
						else
						{
							state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
						}

						// Project scissor/clipping rectangles into framebuffer space
						ImVec4 clipRect;
						clipRect.x = (cmd->ClipRect.x - clipPos.x) * clipScale.x;
						clipRect.y = (cmd->ClipRect.y - clipPos.y) * clipScale.y;
						clipRect.z = (cmd->ClipRect.z - clipPos.x) * clipScale.x;
						clipRect.w = (cmd->ClipRect.w - clipPos.y) * clipScale.y;

						if (clipRect.x < fb_width
							&& clipRect.y < fb_height
							&& clipRect.z >= 0.0f
							&& clipRect.w >= 0.0f)
						{
							uint16 const xx = static_cast<uint16>(bx::max(clipRect.x, 0.0f));
							uint16 const yy = static_cast<uint16>(bx::max(clipRect.y, 0.0f));
							encoder->setScissor(xx, yy
								, static_cast<uint16>(bx::min(clipRect.z, 65535.0f) - xx)
								, static_cast<uint16>(bx::min(clipRect.w, 65535.0f) - yy)
							);

							encoder->setState(state);
							encoder->setTexture(0, texture_uniform, texture_handle);
							encoder->setVertexBuffer(0, &tvb, 0, numVertices);
							encoder->setIndexBuffer(&tib, offset, cmd->ElemCount);
							encoder->submit(a_view_id, shader_program);
						}
					}

					offset += cmd->ElemCount;
				}

				bgfx::end(encoder);
			}
		}

		void ImGui_ImplBGFX_RenderViewportWindow(ImGuiViewport* viewport, void*)
		{
			ImGuiViewportDataBGFX const* viewport_data = static_cast<ImGuiViewportDataBGFX*>(viewport->PlatformUserData);
			bgfx::ViewId const view_id = viewport_data->_view_id;

			if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear))
			{
				bgfx::resetView(view_id);
				bgfx::setViewClear(view_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x55555555);
				bgfx::setViewRect(view_id, 0, 0, bgfx::BackbufferRatio::Equal);
			}

			bool const is_not_main_window = view_id != 0;
			if (is_not_main_window)
			{
				if (bgfx::isValid(viewport_data->_framebuffer_handle))
					bgfx::setViewFrameBuffer(view_id, viewport_data->_framebuffer_handle);
			}

			ImGui_ImplBGFX_RenderDrawData(viewport->DrawData, view_id);
		}

		void ImGui_ImplBGFX_SwapBuffers(ImGuiViewport* /*a_viewport*/, void*)
		{
		}

		bool ImGui_ImplBGFX_CreateFontsTexture()
		{
			ImGuiIO const& io = ImGui::GetIO();
			{
				ImFontConfig config;
				config.FontDataOwnedByAtlas = false;
				config.MergeMode = false;

				//const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
				//_font[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTtf		, sizeof(s_robotoRegularTtf)	, a_font_size		, &config, ranges);
				//_font[ImGui::Font::Mono]	= io.Fonts->AddFontFromMemoryTTF((void*)s_robotoMonoRegularTtf	, sizeof(s_robotoMonoRegularTtf), a_font_size - 3.0f, &config, ranges);

				config.MergeMode = true;
				config.DstFont = DImGui::imgui_rendering_context._font[ImGui::Font::Regular];

				//for (uint32 ii = 0; ii < BX_COUNTOF(DImGui::s_fontRangeMerge); ++ii)
				//{
				//    const DImGui::FontRangeMerge& frm = DImGui::s_fontRangeMerge[ii];
				//    io.Fonts->AddFontFromMemoryTTF((void*)frm.data, static_cast<int>(frm.size), a_font_size - 3.0f, &config, frm.ranges);
				//}
			}

			uint8* data;
			int32 width;
			int32 height;
			io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

			bgfx::TextureHandle& texture_handle = DImGui::imgui_rendering_context._texture_handle;
			texture_handle = bgfx::createTexture2D(
				static_cast<uint16>(width)
				, static_cast<uint16>(height)
				, false
				, 1
				, bgfx::TextureFormat::BGRA8
				, 0
				, bgfx::copy(data, width * height * 4)
			);

			DFW_ASSERT(bgfx::isValid(texture_handle));

			return true;
		}

		void ImGui_ImplBGFX_DestroyFontsTexture()
		{
			bgfx::destroy(DImGui::imgui_rendering_context._texture_handle);
		}

		bool ImGui_ImplBGFX_CreateDeviceObjects()
		{
			bgfx::RendererType::Enum const render_type = bgfx::getRendererType();
			bool const destroy_shaders = true;

			DImGui::imgui_rendering_context._vertex_layout.begin()
				.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
				.end();

			DImGui::imgui_rendering_context._shader_program_handle = bgfx::createProgram(
				bgfx::createEmbeddedShader(DImGui::embedded_imgui_shaders, render_type, "vs_ocornut_imgui")
				, bgfx::createEmbeddedShader(DImGui::embedded_imgui_shaders, render_type, "fs_ocornut_imgui")
				, destroy_shaders
			);

			DImGui::imgui_rendering_context._image_program_handle = bgfx::createProgram(
				bgfx::createEmbeddedShader(DImGui::embedded_imgui_shaders, render_type, "vs_imgui_image")
				, bgfx::createEmbeddedShader(DImGui::embedded_imgui_shaders, render_type, "fs_imgui_image")
				, destroy_shaders
			);

			DImGui::imgui_rendering_context._image_lod_uniform_handle = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
			DImGui::imgui_rendering_context._texture_uniform_handle = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

			DImGui::imgui_rendering_context._are_graphic_devices_initialized = true;

			ImGui_ImplBGFX_CreateFontsTexture();

			return true;
		}

		void ImGui_ImplBGFX_DestroyDeviceObjects()
		{
			bgfx::destroy(DImGui::imgui_rendering_context._shader_program_handle);
			bgfx::destroy(DImGui::imgui_rendering_context._image_program_handle);
			bgfx::destroy(DImGui::imgui_rendering_context._texture_handle);
			bgfx::destroy(DImGui::imgui_rendering_context._texture_uniform_handle);
			bgfx::destroy(DImGui::imgui_rendering_context._image_lod_uniform_handle);
			DImGui::imgui_rendering_context._are_graphic_devices_initialized = false;

			ImGui_ImplBGFX_DestroyFontsTexture();
		}

	} // End of namespace ~ DImGui.

} // End of namespace ~ DFW.
