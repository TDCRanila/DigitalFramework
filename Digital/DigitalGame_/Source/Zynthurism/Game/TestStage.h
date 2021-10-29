#pragma once

#include <Modules/ECS/ECSModule.h>

#include <Zynthurism/GameStageBase.h>

namespace DGame
{
    class TestStage : public GameStageBase
    {
    public:
        TestStage(std::string a_stage_name, bool a_start_disabled = false);
        virtual ~TestStage() = default;

        virtual void RenderImGui() override;

        virtual void Update() override;

        virtual void OnAttached() override;

        virtual void OnRemoved() override;

    private: 

        DECS::ECSModule* ecs;
        DECS::ECSUniverse* _universe;

    };

} // End of namespace ~ DGame.
