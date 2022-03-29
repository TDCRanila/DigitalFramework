#pragma once

#include <CoreSystems/ApplicationEvents.h>
#include <CoreSystems/Memory.h>

#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/Rendering/ViewTarget.h>

namespace DFW
{
    class RenderSystem : public DECS::System::StrictRegistrar<RenderSystem>
    {
    public:
        RenderSystem();
        ~RenderSystem() = default;

        virtual void Init() override;
        virtual void Terminate() override;

        virtual void Update(DECS::Universe& a_universe) override;

    private:
        void OnWindowResizeEvent(WindowResizeEvent const& a_window_event);

        SharedPtr<DRender::ViewTarget const> _view_target;
        int32 window_width;
        int32 window_height;

    };

} // End of namespace ~ DFW.
