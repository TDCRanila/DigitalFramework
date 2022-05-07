#pragma once

#include <GameWorld/GameEvents.h>
#include <GameWorld/Camera/CameraEvents.h>

#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/Rendering/ViewTarget.h>
#include <Modules/Rendering/ShaderProgram.h>
#include <Modules/Rendering/Uniform.h>

#include <CoreSystems/ApplicationEvents.h>
#include <CoreSystems/Memory.h>

namespace DFW
{
    struct SpriteBatch;

    class SpriteSystem final : public DECS::System::StrictRegistrar<SpriteSystem>
    {
    public:
        SpriteSystem();
        virtual ~SpriteSystem() = default;

    private:
        virtual void Init() override;
        virtual void Terminate() override;

        virtual void Update(DECS::Universe& a_universe) override;

        void FlushSpriteBatch(SpriteBatch const& a_sprite_batch, DRender::ViewTarget const& a_view_target);

        void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);
        void OnCameraNewActiveEvent(CameraNewActiveEvent const& a_camera_event);
        
    private:
        SharedPtr<DRender::ViewTarget const> _view_target;
        CameraComponent* _rendering_camera;

        SharedPtr<DRender::Uniform> _texture_sampler_uniform;
        DFW::SharedPtr<DFW::DRender::ShaderProgram> _program_ptr;

        int32 window_width;
        int32 window_height;

    };

} // End of namespace ~ DFW.
