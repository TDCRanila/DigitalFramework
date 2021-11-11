#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Utility.h>

#include <Defines/MathDefines.h>

namespace DFW
{
    namespace DImGui
    {
        ImGuiRenderingContext   imgui_rendering_context;
        GLFWwindow* main_window = nullptr;
        bgfx::ViewId            main_window_id = 0;

        bool				    mouse_just_pressed[ImGuiMouseButton_COUNT] = { false };
        bool                    installed_callbacks = false;
        bool                    want_update_monitors = true;

        GLFWmousebuttonfun      prev_user_callback_mousebutton = nullptr;
        GLFWscrollfun           prev_user_callback_scroll = nullptr;
        GLFWkeyfun              prev_user_callback_key = nullptr;
        GLFWcharfun             prev_user_callback_char = nullptr;
        GLFWmonitorfun          prev_user_callback_monitor = nullptr;

        ImGuiViewportDataBGFX::ImGuiViewportDataBGFX()
            : _window(nullptr)
            , _window_owned(false)
            , _ignore_window_size_event_frame(DMath::GetMinValueOfType<decltype(_ignore_window_size_event_frame)>())
            , _ignore_window_pos_event_frame(DMath::GetMinValueOfType<decltype(_ignore_window_pos_event_frame)>())
            , _view_id(bgfx::kInvalidHandle)
        {
            _framebuffer_handle.idx = bgfx::kInvalidHandle;
        }

        ImGuiRenderingContext::ImGuiRenderingContext()
            : _font({ nullptr })
            , _shader_program_handle(BGFX_INVALID_HANDLE)
            , _image_program_handle(BGFX_INVALID_HANDLE)
            , _texture_handle(BGFX_INVALID_HANDLE)
            , _texture_uniform_handle(BGFX_INVALID_HANDLE)
            , _image_lod_uniform_handle(BGFX_INVALID_HANDLE)
            , _viewport_id_counter(-1)
            , _are_graphic_devices_initialized(false)
        {
        }

        bgfx::ViewId ImGui_ImplBGFX_AllocateViewportID()
        {
            if (DImGui::imgui_rendering_context._free_viewport_ids.empty())
            {
                return ++DImGui::imgui_rendering_context._viewport_id_counter;
            }
            else
            {
                bgfx::ViewId const free_id = DImGui::imgui_rendering_context._free_viewport_ids.back();
                DImGui::imgui_rendering_context._free_viewport_ids.pop_back();
                return free_id;
            }
        }

        void ImGui_ImplBGFX_FreeViewportID(bgfx::ViewId const a_viewport_id)
        {
            DImGui::imgui_rendering_context._free_viewport_ids.emplace_back(a_viewport_id);
        }

    } // End of namespace ~ DImGui.

} // End of namespace ~ DFW.

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

        tex.s.handle = a_handle;
        tex.s.flags = a_flags;
        tex.s.mip = a_mip;
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

} // End of namespace ~ ImGui.
