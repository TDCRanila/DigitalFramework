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

        bgfx::RendererType::Enum render_type = bgfx::getRendererType();
        const char* render_name = bgfx::getRendererName(render_type);
        
        ImGuiIO& io = ImGui::GetIO();
        std::string backend_renderer_name = "bgfx-";
        backend_renderer_name += std::string(render_name);

        io.BackendRendererName = backend_renderer_name.c_str();
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
        }

        bool destroy_shaders = true;
        bgfx::ProgramHandle shader_program_handle;
        bgfx::ProgramHandle image_shader_program_handle;
        bgfx::UniformHandle texture_uniform_handle;
        bgfx::UniformHandle image_lod_uniform_handle;

        shader_program_handle = bgfx::createProgram(
            bgfx::createEmbeddedShader(DImGui::s_embeddedShaders, render_type, "vs_ocornut_imgui")
            , bgfx::createEmbeddedShader(DImGui::s_embeddedShaders, render_type, "fs_ocornut_imgui")
            , destroy_shaders
        );

        image_shader_program_handle = bgfx::createProgram(
        bgfx::createEmbeddedShader(DImGui::s_embeddedShaders, render_type, "vs_imgui_image")
        , bgfx::createEmbeddedShader(DImGui::s_embeddedShaders, render_type, "fs_imgui_image")
        , destroy_shaders);

        image_lod_uniform_handle    = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
        texture_uniform_handle      = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

        DImGui::imgui_context._last = 5;

        for (int32 view_id_it = 0; view_id_it <= 255; ++view_id_it)
        {
            DImGui::imgui_context.u_imageLodEnabled[view_id_it] = image_lod_uniform_handle;
            DImGui::imgui_context.s_texs[view_id_it] = texture_uniform_handle;

            DImGui::imgui_context._programs[view_id_it] = shader_program_handle;

            DImGui::imgui_context._image_programs[view_id_it] = image_shader_program_handle;

            DImGui::imgui_context._layouts[view_id_it].begin()
                .add(bgfx::Attrib::Position , 2, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0   , 4, bgfx::AttribType::Uint8, true)
                .end();
        }

        bx::memSet(DImGui::imgui_context._framebuffer_handles, 0xff, sizeof(DImGui::imgui_context._framebuffer_handles));

        return true;
	}

	void ImGui_ImplBGFX_ShutdownGraphics()
	{
        //ImGui_ImplBGFX_DestroyDeviceObjects();

        for (int32 view_id_it = 0; view_id_it < 255; ++view_id_it)
        {
            bgfx::destroy(DImGui::imgui_context._framebuffer_handles[view_id_it]);
            bgfx::destroy(DImGui::imgui_context.s_texs[view_id_it]);
            bgfx::destroy(DImGui::imgui_context._texture_handles[view_id_it]);
            bgfx::destroy(DImGui::imgui_context._image_programs[view_id_it]);
            bgfx::destroy(DImGui::imgui_context._programs[view_id_it]);
            bgfx::destroy(DImGui::imgui_context.u_imageLodEnabled[view_id_it]);
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
        //a_draw_data->OwnerViewport->PlatformRequestMove = true;
        //a_draw_data->OwnerViewport->PlatformRequestResize = true;

        //DFW_INFOLOG("ViewID: " << a_view_id << " DrawData DisplayPos: " << a_draw_data->DisplayPos.x << " - " << a_draw_data->DisplayPos.y << " --- " << a_draw_data->DisplaySize.x << " - " << a_draw_data->DisplaySize.y);
        //DFW_INFOLOG("ViewID: " << a_view_id << " DrawData DisplaySize:);

        int left, top, right, bottom;
        glfwGetWindowFrameSize((GLFWwindow*)a_draw_data->OwnerViewport->PlatformHandle, &left, &top, &right, &bottom);
        int height, width;
        glfwGetFramebufferSize((GLFWwindow*)a_draw_data->OwnerViewport->PlatformHandle, &height, &width);

        ImGuiIO& io     = ImGui::GetIO();
        uint16 draw_data_fb_width  = static_cast<uint16>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
        uint16 draw_data_fb_height = static_cast<uint16>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
        if (draw_data_fb_width <= 0 || draw_data_fb_height <= 0) 
        {
            return;
        }

        a_draw_data->ScaleClipRects(io.DisplayFramebufferScale);

        bgfx::setViewName(a_view_id, "ImGui" + a_view_id);
        bgfx::setViewMode(a_view_id, bgfx::ViewMode::Sequential);

        // Setup viewport, orthographic projection matrix
        const bgfx::Caps* caps = bgfx::getCaps();
        float32 ortho[16];

        float32 L = a_draw_data->DisplayPos.x;
        float32 R = a_draw_data->DisplayPos.x + a_draw_data->DisplaySize.x;
        float32 T = a_draw_data->DisplayPos.y;
        float32 B = a_draw_data->DisplayPos.y + a_draw_data->DisplaySize.y;
        //if (true) { float tmp = T; T = B; B = tmp; } // Swap top and bottom if origin is upper left
        
        bgfx::setViewRect(a_view_id, 0, 0, draw_data_fb_width, draw_data_fb_height);
        bx::mtxOrtho(ortho, L, R, T, B, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
        bgfx::setViewTransform(a_view_id, NULL, ortho);;

        // Render command lists
        for (int32 it = 0, num = a_draw_data->CmdListsCount; it < num; ++it)
        {
            const ImDrawList* drawList              = a_draw_data->CmdLists[it];
            const bgfx::VertexLayout& vertex_layout = imgui_context._layouts[a_view_id];

            uint32 numVertices  = static_cast<uint32>(drawList->VtxBuffer.size());
            uint32 numIndices   = static_cast<uint32>(drawList->IdxBuffer.size());

            if ((numVertices != bgfx::getAvailTransientVertexBuffer(numVertices, vertex_layout)) || (numIndices != bgfx::getAvailTransientIndexBuffer(numIndices)))
            {
                // not enough space in transient buffer, quit drawing the rest...
                break;
            }

            bgfx::TransientVertexBuffer tvb;
            bgfx::TransientIndexBuffer tib;
            bgfx::allocTransientVertexBuffer(&tvb, numVertices, vertex_layout);
            bgfx::allocTransientIndexBuffer(&tib, numIndices);

            ImDrawVert* verts   = (ImDrawVert*)tvb.data;
            ImDrawIdx* indices  = (ImDrawIdx*)tib.data;
            bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));
            bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

            uint32 index_buffer_offset = 0;
            for (const ImDrawCmd& cmd : drawList->CmdBuffer)
            {
                if (cmd.UserCallback)
                {
                    cmd.UserCallback(drawList, &cmd);
                }
                else if (0 != cmd.ElemCount)
                {
                    uint64 state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;

                    bgfx::TextureHandle texture_handle = DImGui::imgui_context._texture_handles[a_view_id];
                    bgfx::ProgramHandle shader_program = DImGui::imgui_context._programs[a_view_id];

                    bool is_texture = NULL != cmd.TextureId;
                    if (is_texture)
                    {
                        union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8 flags; uint8 mip; } s; } texture = { cmd.TextureId };
                        state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags) ? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA) : BGFX_STATE_NONE;
                        texture_handle = texture.s.handle;
                        if (0 != texture.s.mip)
                        {
                            const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
                            bgfx::setUniform(DImGui::imgui_context.u_imageLodEnabled[a_view_id], lodEnabled);
                            shader_program = DImGui::imgui_context._image_programs[a_view_id];
                        }
                    }
                    else
                    {
                        state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
                    }

                    ImVec2 clip_off     = a_draw_data->DisplayPos;         // (0,0) unless using multi-viewports
                    ImVec2 clip_scale   = a_draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

                    ImVec4 clip_rect;
                    clip_rect.x = (cmd.ClipRect.x - clip_off.x) * clip_scale.x;
                    clip_rect.y = (cmd.ClipRect.y - clip_off.y) * clip_scale.y;
                    clip_rect.z = (cmd.ClipRect.z - clip_off.x) * clip_scale.x;
                    clip_rect.w = (cmd.ClipRect.w - clip_off.y) * clip_scale.y;

                    if (clip_rect.x < draw_data_fb_width && clip_rect.y < draw_data_fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                    {
                        const uint16 xx = static_cast<uint16>(clip_rect.x);
                        const uint16 yy = static_cast<uint16>(draw_data_fb_height - clip_rect.w);
                        const uint16 ww = static_cast<uint16>(clip_rect.z - clip_rect.x);
                        const uint16 hh = static_cast<uint16>(clip_rect.w - clip_rect.y);
                        bgfx::setViewScissor(a_view_id, xx, yy, ww, hh);

                        bgfx::setState(state);
                        bgfx::setTexture(0, DImGui::imgui_context.s_texs[a_view_id], texture_handle);
                        bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
                        bgfx::setIndexBuffer(&tib, index_buffer_offset, cmd.ElemCount);
                        bgfx::submit(a_view_id, shader_program);
                    }
                }

                index_buffer_offset += cmd.ElemCount;
            }
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
