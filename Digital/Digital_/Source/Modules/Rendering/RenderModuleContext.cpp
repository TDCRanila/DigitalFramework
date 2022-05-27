#include <Modules/Rendering/RenderModuleContext.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <Modules/Rendering/ViewTarget.h>

#include <bgfx/platform.h>

namespace DFW
{
    namespace DRender
    {
        RenderModuleContext::RenderModuleContext()
        {
            // Set default initialization settings. 
            _bgfx_init_settings.type                = bgfx::RendererType::Count;
            _bgfx_init_settings.resolution.width    = DWindow::DFW_DEFAULT_WINDOW_WIDTH;
            _bgfx_init_settings.resolution.height   = DWindow::DFW_DEFAULT_WINDOW_HEIGHT;
            _bgfx_init_settings.resolution.reset    = BGFX_RESET_VSYNC;
        }

        void RenderModuleContext::InitRenderModuleContext()
        {
            // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
            bgfx::renderFrame();

            SharedPtr<DWindow::WindowInstance> main_window_ptr = CoreService::GetWindowSystem()->GetMainWindow();
            DFW_ASSERT(main_window_ptr, "Pointer to the main window is invalid, window mangement might not have been initialised.");

            bgfx::PlatformData platform_data;
            platform_data.nwh = CoreService::GetWindowSystem()->GetMainWindowPWH();
            bgfx::setPlatformData(platform_data);

            bgfx::init(_bgfx_init_settings);

            // Register Event Callbacks.
            CoreService::GetMainEventHandler()->RegisterCallback<WindowResizeEvent, &RenderModuleContext::OnWindowResizeEvent>(this);
        }

        void RenderModuleContext::TerminateRenderModuleContext()
        {
            // Unregister Event Callbacks.
            CoreService::GetMainEventHandler()->UnregisterCallback<WindowResizeEvent, &RenderModuleContext::OnWindowResizeEvent>(this);

            bgfx::shutdown();
        }
                          
        void RenderModuleContext::BeginFrame(SharedPtr<ViewTarget const> const& a_main_viewtarget)
        {
            bgfx::setViewRect(*a_main_viewtarget, 0, 0, bgfx::BackbufferRatio::Equal);
            bgfx::setViewClear(*a_main_viewtarget, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
            bgfx::touch(*a_main_viewtarget);
        }

        void RenderModuleContext::EndFrame()
        {
        }
                          
        void RenderModuleContext::RenderFrame()
        {
            // Advance to next frame. Process submitted rendering primitives.
            bgfx::frame();
        }

        void RenderModuleContext::SubmitMesh()
        {
        }
        
        void RenderModuleContext::SubmitSprite()
        {
        }
          
        bgfx::RendererType::Enum RenderModuleContext::GetRenderAPIType() const
        {
            return bgfx::getRendererType();
        }

        void RenderModuleContext::ChangeRenderAPI(bgfx::RendererType::Enum a_render_type)
        {
            // It is possible to change render api during runtime, but that might cause issues with some hardware.
            // Calling this function will most likely result in a runtime error if this fix is not applied.
            // issue: https://github.com/bkaradzic/bgfx/pull/2395
            // fix: https://github.com/bkaradzic/bgfx/pull/1837/commits/ab13e6281c12b3916883be33e8a15890313443d3

            // TODO Potential issue with non-dx11/dx12 APIs that don't seem to render properly after changing.
            // Needs further investigation.

            if (_bgfx_init_settings.type == a_render_type)
                return;

            TerminateRenderModuleContext();

            _bgfx_init_settings.type = a_render_type;

            InitRenderModuleContext();

            CoreService::GetMainEventHandler()->InstantBroadcast<RendererAPIChanged>();
        }

        void RenderModuleContext::ChangeGraphicsSettings(uint32 const a_bgfx_reset_flags)
        {
            // Toggle a flag depending on the input.
            _bgfx_init_settings.resolution.reset = _bgfx_init_settings.resolution.reset ^ a_bgfx_reset_flags;
            bgfx::reset(_bgfx_init_settings.resolution.width, _bgfx_init_settings.resolution.height, _bgfx_init_settings.resolution.reset);
        }

        void RenderModuleContext::Debug_DrawBasicRenderInfo() const
        {
            // Enable stats or debug text.
            static bool switch_debug_stats  = false;
            static bool show_debug_info     = false;
            static auto bgfx_debug_config   = BGFX_DEBUG_TEXT;

            static auto input_system_ptr    = CoreService::GetInputSystem();
            static auto window_system_ptr   = CoreService::GetWindowSystem();
            static auto main_window_ptr     = window_system_ptr->GetMainWindow();

            bool const key_f1_pressed = input_system_ptr->IsKeyReleased(DInput::DKey::F1);
            if (key_f1_pressed)
            {
                bool const key_shift_pressed = 
                        input_system_ptr->IsKeyDown(DInput::DKey::LEFT_SHIFT) 
                    ||  input_system_ptr->IsKeyDown(DInput::DKey::RIGHT_SHIFT);

                if (show_debug_info && key_shift_pressed)
                {
                    bgfx_debug_config   = switch_debug_stats ? BGFX_DEBUG_TEXT : BGFX_DEBUG_STATS;
                    switch_debug_stats  = switch_debug_stats ? false : true;
                    bgfx::setDebug(bgfx_debug_config);
                }
                else if (!key_shift_pressed)
                {
                    bgfx::setDebug(show_debug_info ? BGFX_DEBUG_NONE : bgfx_debug_config);
                    show_debug_info = show_debug_info ? false : true;
                }
            }

            if (show_debug_info)
            {
                DWindow::WindowDimension const& window_dimension = main_window_ptr->_window_dimension;

                // bgfx debug Window Information
                bgfx::dbgTextClear();

                bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to hide debug information. Press Shift+F1 to toggle stats.");

                bgfx::dbgTextPrintf(0, 1, 0x0f, "Digital Info: ");

                std::string is_window_focussed_string = main_window_ptr->is_focussed ? "MainWindow is focussed." : "MainWindow is unfocussed.";
                bgfx::dbgTextPrintf(15, 1, 0x0f, is_window_focussed_string.c_str());

                bgfx::dbgTextPrintf(41, 1, 0x0f, "fW:%d x fH:%d.", window_dimension._current_frame_width, window_dimension._current_frame_height);
                bgfx::dbgTextPrintf(61, 1, 0x0f, "W:%d x H:%d.", window_dimension._current_width, window_dimension._current_height);

                const bgfx::Stats* stats = bgfx::getStats();
                bgfx::dbgTextPrintf(0, 2, 0x0f, "Bgfx backbuffer %dW x %dH in px, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

                bgfx::dbgTextPrintf(0, 3, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
                bgfx::dbgTextPrintf(0, 4, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
                bgfx::dbgTextPrintf(0, 5, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
            }
        }

        void RenderModuleContext::OnWindowResizeEvent(WindowResizeEvent const& a_window_event)
        {
            bgfx::reset(a_window_event.new_width, a_window_event.new_height);
            bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
        }

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
