#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Rendering.h>

#include <bgfx/embedded_shader.h>
#include <bx/bx.h>
#include <bx/allocator.h>
#include <bx/math.h>

#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Utility.h>

#include <glfw/glfw3.h>

namespace DImGui
{
	
	bool ImGui_ImplBGFX_InitGraphics()
	{
        bgfx::RendererType::Enum const render_type   = bgfx::getRendererType();
        const char* render_name                     = bgfx::getRendererName(render_type);
        std::string const backend_renderer_name     = std::string("bgfx-") + render_name;
        
        ImGuiIO& io = ImGui::GetIO();

        io.BackendRendererName = backend_renderer_name.c_str();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
        }

        bool const destroy_shaders = true;
        bgfx::ProgramHandle shader_program_handle;
        bgfx::ProgramHandle image_shader_program_handle;
        bgfx::UniformHandle texture_uniform_handle;
        bgfx::UniformHandle image_lod_uniform_handle;

        shader_program_handle = bgfx::createProgram(
                bgfx::createEmbeddedShader(DImGui::s_embeddedShaders, render_type, "vs_ocornut_imgui")
            ,   bgfx::createEmbeddedShader(DImGui::s_embeddedShaders, render_type, "fs_ocornut_imgui")
            ,   destroy_shaders
        );

        image_shader_program_handle = bgfx::createProgram(
                bgfx::createEmbeddedShader(DImGui::s_embeddedShaders, render_type, "vs_imgui_image")
            ,   bgfx::createEmbeddedShader(DImGui::s_embeddedShaders, render_type, "fs_imgui_image")
            ,   destroy_shaders
        );

        image_lod_uniform_handle    = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
        texture_uniform_handle      = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

        // Construct different buffers for viewports.
        int8 const num_framebuffers = DImGui::imgui_context._num_framebuffers;
        for (int32 index = 0; index < num_framebuffers; ++index)
        {
            DImGui::imgui_context._layouts[index].begin()
                .add(bgfx::Attrib::Position , 2, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0   , 4, bgfx::AttribType::Uint8, true)
                .end();

            DImGui::imgui_context._programs[index]                     = shader_program_handle;
            DImGui::imgui_context._image_programs[index]               = image_shader_program_handle;

            DImGui::imgui_context._texture_uniform_handles[index]      = texture_uniform_handle;
            DImGui::imgui_context._image_lod_uniform_handles[index]    = image_lod_uniform_handle;
        }

        bx::memSet(DImGui::imgui_context._framebuffer_handles, 0xff, num_framebuffers);

        return true;
	}

	void ImGui_ImplBGFX_ShutdownGraphics()
	{
        //ImGui_ImplBGFX_DestroyDeviceObjects();

        int8 const num_framebuffers = DImGui::imgui_context._num_framebuffers;
        for (int32 index = 0; index < num_framebuffers; ++index)
        {
            bgfx::destroy(DImGui::imgui_context._framebuffer_handles[index]);
            bgfx::destroy(DImGui::imgui_context._texture_handles[index]);
            bgfx::destroy(DImGui::imgui_context._programs[index]);
            bgfx::destroy(DImGui::imgui_context._image_programs[index]);
            bgfx::destroy(DImGui::imgui_context._texture_uniform_handles[index]);
            bgfx::destroy(DImGui::imgui_context._image_lod_uniform_handles[index]);
        }

        DImGui::imgui_context._allocator = nullptr;
	}

	void ImGui_ImplBGFX_NewFrameGraphics()
	{
        //if (!g_ShaderHandle)
        //    ImGui_ImplOpenGL3_CreateDeviceObjects();
	}

	void ImGui_ImplBGFX_RenderDrawData(ImDrawData* a_draw_data, bgfx::ViewId a_view_id)
	{
	       // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		int fb_width = (int)(a_draw_data->DisplaySize.x * a_draw_data->FramebufferScale.x);
		int fb_height = (int)(a_draw_data->DisplaySize.y * a_draw_data->FramebufferScale.y);
		if (fb_width <= 0 || fb_height <= 0)
			return;
	
		bgfx::setViewName(a_view_id, "ImGui");
		bgfx::setViewMode(a_view_id, bgfx::ViewMode::Sequential);
	
		const bgfx::Caps* caps = bgfx::getCaps();
		{
			float ortho[16];
			float x = a_draw_data->DisplayPos.x;
			float y = a_draw_data->DisplayPos.y;
			float width = a_draw_data->DisplaySize.x;
			float height = a_draw_data->DisplaySize.y;
	
			bx::mtxOrtho(ortho, x, x + width, y + height, y, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
			bgfx::setViewTransform(a_view_id, NULL, ortho);
			bgfx::setViewRect(a_view_id, 0, 0, uint16_t(width), uint16_t(height) );
		}
	
		const ImVec2 clipPos   = a_draw_data->DisplayPos;       // (0,0) unless using multi-viewports
		const ImVec2 clipScale = a_draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)
	
		// Render command lists
		for (int32_t index = 0, num = a_draw_data->CmdListsCount; index < num; ++index)
		{
			bgfx::TransientVertexBuffer tvb;
			bgfx::TransientIndexBuffer tib;
	
			ImDrawList const* drawList = a_draw_data->CmdLists[index];
			uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
			uint32_t numIndices  = (uint32_t)drawList->IdxBuffer.size();
	
			auto checkAvailTransientBuffers = [](uint32_t _numVertices, const bgfx::VertexLayout& _layout, uint32_t _numIndices)
			{
				return _numVertices == bgfx::getAvailTransientVertexBuffer(_numVertices, _layout)
					&& (0 == _numIndices || _numIndices == bgfx::getAvailTransientIndexBuffer(_numIndices))
					;
			};
	
			bgfx::VertexLayout const& vertex_layout = imgui_context._layouts[a_view_id];
			if (!checkAvailTransientBuffers(numVertices, vertex_layout, numIndices) )
			{
				// not enough space in transient buffer just quit drawing the rest...
				break;
			}
	
			bgfx::allocTransientVertexBuffer(&tvb, numVertices, vertex_layout);
			bgfx::allocTransientIndexBuffer(&tib, numIndices, sizeof(ImDrawIdx) == 4);
	
			ImDrawVert* verts = (ImDrawVert*)tvb.data;
			bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert) );
	
			ImDrawIdx* indices = (ImDrawIdx*)tib.data;
			bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx) );
	
			bgfx::Encoder* encoder = bgfx::begin();
	
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
	
					bgfx::TextureHandle texture_handle	= DImGui::imgui_context._texture_handles[a_view_id];
					bgfx::ProgramHandle shader_program	= DImGui::imgui_context._programs[a_view_id];
					bgfx::ProgramHandle image_program	= DImGui::imgui_context._image_programs[a_view_id];
					bgfx::UniformHandle image_lod		= DImGui::imgui_context._image_lod_uniform_handles[a_view_id];
					bgfx::UniformHandle texture_uniform	= DImGui::imgui_context._texture_uniform_handles[a_view_id];
	
					if (NULL != cmd->TextureId)
					{
						union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; } texture = { cmd->TextureId };
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
	
					if (clipRect.x <  fb_width
					&&  clipRect.y <  fb_height
					&&  clipRect.z >= 0.0f
					&&  clipRect.w >= 0.0f)
					{
						const uint16_t xx = uint16_t(bx::max(clipRect.x, 0.0f) );
						const uint16_t yy = uint16_t(bx::max(clipRect.y, 0.0f) );
						encoder->setScissor(xx, yy
								, uint16_t(bx::min(clipRect.z, 65535.0f)-xx)
								, uint16_t(bx::min(clipRect.w, 65535.0f)-yy)
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
			
    void ImGui_ImplBGFX_RenderWindow(ImGuiViewport* viewport, void*)
    {
        ImGuiViewportDataBGFX* data = (ImGuiViewportDataBGFX*)viewport->PlatformUserData;
        const bgfx::ViewId view_id  = data->_view_id;
        if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear))
        {
            bgfx::resetView(view_id);
            bgfx::setViewClear(view_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x55555555);
            bgfx::setViewRect(view_id, 0, 0, bgfx::BackbufferRatio::Equal);
        }

        bool is_not_main_window = view_id != 0;
        if (is_not_main_window)
        {
            if (bgfx::isValid(DImGui::imgui_context._framebuffer_handles[view_id]))
                bgfx::setViewFrameBuffer(view_id, DImGui::imgui_context._framebuffer_handles[view_id]);
        }

        ImGui_ImplBGFX_RenderDrawData(viewport->DrawData, view_id);
    }

    void ImGui_ImplBGFX_SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height, float& a_ortho_return)
    {

    }

    void ImGui_ImplBGFX_SwapBuffers(ImGuiViewport* viewport, void*)
    {

    }

} // End of namespace ~ DImGui.
