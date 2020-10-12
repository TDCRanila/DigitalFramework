#include <CoreSystems/ImGui/ImGui_Impl_BGFX_Utility.h>

namespace DImGui
{
    OcornutImguiContext imgui_context;
    
    GLFWwindow* main_window = nullptr;
    
    bool				 mouse_just_pressed[ImGuiMouseButton_COUNT] = {};
    
    bool                 installed_callbacks    = false;
    bool                 want_update_monitors   = true;

    GLFWmousebuttonfun   prev_user_callback_mousebutton = NULL;
    GLFWscrollfun        prev_user_callback_scroll      = NULL;
    GLFWkeyfun           prev_user_callback_key         = NULL;
    GLFWcharfun          prev_user_callback_char        = NULL;
    GLFWmonitorfun       prev_user_callback_monitor     = NULL;

    ImGuiViewportDataBGFX::ImGuiViewportDataBGFX()
        : _window(NULL)
        , _window_owned(false)
        , _ignore_window_size_event_frame(-1)
        , _ignore_window_pos_event_frame(-1)
    {
    }

    ImGuiViewportDataBGFX::~ImGuiViewportDataBGFX()
    {
        IM_ASSERT(_window == NULL);
    }

    bool ImGui_ImplBGFX_AddFonts()
    {
        ImGuiIO& io = ImGui::GetIO();

        uint8* data;
        int32 width;
        int32 height;
        {
            ImFontConfig config;
            config.FontDataOwnedByAtlas = false;
            config.MergeMode = false;

            const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
            //_font[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTtf		, sizeof(s_robotoRegularTtf)	, a_font_size		, &config, ranges);
            //_font[ImGui::Font::Mono]	= io.Fonts->AddFontFromMemoryTTF((void*)s_robotoMonoRegularTtf	, sizeof(s_robotoMonoRegularTtf), a_font_size - 3.0f, &config, ranges);

            config.MergeMode = true;
            config.DstFont = DImGui::imgui_context._font[ImGui::Font::Regular];

            for (uint32 ii = 0; ii < BX_COUNTOF(DImGui::s_fontRangeMerge); ++ii)
            {
                const DImGui::FontRangeMerge& frm = DImGui::s_fontRangeMerge[ii];
                //io.Fonts->AddFontFromMemoryTTF((void*)frm.data, static_cast<int>(frm.size), a_font_size - 3.0f, &config, frm.ranges);
            }
        }

        io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
        for (int32 view_id_it = 0; view_id_it < 255; ++view_id_it)
        {
            DImGui::imgui_context._texture_handles[view_id_it] = bgfx::createTexture2D(static_cast<uint16>(width), static_cast<uint16>(height), false, 1, bgfx::TextureFormat::BGRA8, 0, bgfx::copy(data, width * height * 4));
        }

        // TODO - proper return and checking if something can go wrong
        return true;
    }

} // End of namespace ~ DImGui.

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
