#include <Modules/Rendering/RenderModule_Impl_BGFX.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/DUID.h>
#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/Input/InputManagement.h>
#include <CoreSystems/ImGui/ImGuiLayer.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

namespace DFW
{
    namespace DRender
    {
        void RenderModuleBGFX::InitRenderModule()
        {
            // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
            bgfx::renderFrame();

            SharedPtr<DWindow::WindowInstance> main_window_ptr = CoreService::GetWindowSystem()->GetMainWindow();
            DFW_ASSERT(main_window_ptr, "Pointer to the main window is invalid, window mangement might not have been initialised.");

            bgfx::PlatformData platform_data;
            platform_data.nwh = CoreService::GetWindowSystem()->GetMainWindowPWH();
            bgfx::setPlatformData(platform_data);

            bgfx::Init bgfx_init;
            bgfx_init.type              = bgfx::RendererType::Count; // Auto Choose Renderer
            bgfx_init.resolution.width  = main_window_ptr->_window_dimension._current_width;
            bgfx_init.resolution.height = main_window_ptr->_window_dimension._current_height;
            bgfx_init.resolution.reset  = BGFX_RESET_VSYNC;
            bgfx::init(bgfx_init);

            // Register Event Callbacks.
            CoreService::GetMainEventHandler()->RegisterCallback<WindowResizeEvent, &RenderModuleBGFX::OnWindowResizeEvent>(this);
        }

        void RenderModuleBGFX::TerminateRenderModule()
        {
            // Unregister Event Callbacks.
            CoreService::GetMainEventHandler()->UnregisterCallback<WindowResizeEvent, &RenderModuleBGFX::OnWindowResizeEvent>(this);

            bgfx::shutdown();
        }
             
        void RenderModuleBGFX::RenderFrame()
        {
            // Advance to next frame. Process submitted rendering primitives.
            bgfx::frame();
        }
             
        void RenderModuleBGFX::BeginFrame()
        {
            bgfx::ViewId main_window = 0;
            bgfx::setViewRect(main_window, 0, 0, bgfx::BackbufferRatio::Equal);
            bgfx::setViewClear(main_window, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x33333333);
        }

        void RenderModuleBGFX::EndFrame()
        {
        }
                          
        void RenderModuleBGFX::SubmitMesh()
        {
        }
        
        void RenderModuleBGFX::SubmitSprite()
        {
        }
          
        void RenderModuleBGFX::OnWindowResizeEvent(WindowResizeEvent const& a_window_event)
        {
            bgfx::reset(a_window_event.new_width, a_window_event.new_height);
            bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
        }

        void RenderModuleBGFX::Debug_DrawBasicRenderInfo() const
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

                //// This dummy draw call is here to make sure that view 0 is cleared
                //// if no other draw calls are submitted to view 0.
                //bgfx::touch(0);

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

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
