#pragma once

#include <GameWorld/GameEvents.h>
#include <GameWorld/Camera/CameraEvents.h>

#include <CoreSystems/ApplicationEvents.h>
#include <CoreSystems/Memory.h>

#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/Rendering/ViewTarget.h>
#include <Modules/Rendering/Uniform.h>

namespace DFW
{
    struct CameraComponent;

    class RenderSystem : public DECS::System::StrictRegistrar<RenderSystem>
    {
    public:
        RenderSystem();
        ~RenderSystem() = default;

    private:
        virtual void Init() override;
        virtual void Terminate() override;

        virtual void Update(DECS::Universe& a_universe) override;

        void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);
        void OnCameraNewActiveEvent(CameraNewActiveEvent const& a_camera_event);

        SharedPtr<DRender::ViewTarget const> _view_target;
        CameraComponent* _rendering_camera;

        SharedPtr<DRender::Uniform> _texture_sampler_uniform;

        int32 window_width;
        int32 window_height;

    };

} // End of namespace ~ DFW.
