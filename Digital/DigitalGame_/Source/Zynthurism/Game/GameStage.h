#pragma once

#include <Modules/ECS/ECSModule.h>

#include <Zynthurism/GameStageBase.h>

namespace DGame
{
    class GameTestStage : public GameStageBase
    {
    public:
        GameTestStage(std::string a_stage_name, bool a_start_disabled = false);
        virtual ~GameTestStage() = default;

        virtual void RenderImGui() override;

        virtual void Update() override;

        virtual void OnAttached() override;

        virtual void OnRemoved() override;

    private: 

        DECS::ECSModule ecs;

    };

} // End of namespace ~ DGame.
