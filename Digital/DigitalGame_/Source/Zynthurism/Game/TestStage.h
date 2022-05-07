#pragma once

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Objects/ECSEntity.h>

#include <Zynthurism/GameStageBase.h>

namespace DGame
{
    class TestStage : public GameStageBase
    {
    public:
        TestStage(std::string a_stage_name, bool a_start_disabled = false);
        virtual ~TestStage() = default;

        virtual void Update() override;

        virtual void OnAttached() override;
        virtual void OnRemoved() override;

    private:
        DFW::DECS::ECSModule* ecs;
        DFW::DECS::Universe* _universe;

        DFW::Entity main_entity;
        DFW::Entity camera_entity;

    };

} // End of namespace ~ DGame.
