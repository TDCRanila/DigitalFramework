#pragma once

#include <GameWorld/GameEvents.h>
#include <GameWorld/Camera/CameraEvents.h>

#include <Modules/ECS/System.h>

#include <CoreSystems/ApplicationEvents.h>

namespace DFW
{
    struct CameraComponent;

    namespace DRender
    {
        struct ViewTarget;
        struct RenderTarget;
        struct ShaderProgram;
        struct Uniform;

    } // End of namespace ~ DRender.

    class BaseRenderSystem : public DECS::System
    {
    public:
        BaseRenderSystem();
        virtual ~BaseRenderSystem() = default;

        void RenderToRenderTarget(SharedPtr<DRender::RenderTarget const> const& a_render_target);
        void RenderToDefault();

    protected:
        void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);
        void OnCameraNewActiveEvent(CameraNewActiveEvent const& a_camera_event);

    protected:
        SharedPtr<DRender::ViewTarget const> _view_target;
        SharedPtr<DRender::RenderTarget const> _render_target;
        DFW::SharedPtr<DRender::ShaderProgram> _program_ptr;
        SharedPtr<DRender::Uniform> _texture_sampler_uniform;

        CameraComponent* _rendering_camera;
                
        int32 window_width;
        int32 window_height;

    };
}
