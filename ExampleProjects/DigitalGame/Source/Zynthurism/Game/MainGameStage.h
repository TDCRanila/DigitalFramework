#pragma once

#include <DFW/Modules/ECS/ECSModule.h>
#include <DFW/Modules/ECS/Entity.h>

#include <DFW/GameWorld/Controller/CharacterController.h>

#include <DFW/CoreSystems/Stage/Stage.h>

namespace DGame
{
    class MainGameStage : public DFW::StageBase
    {
    public:
        MainGameStage(std::string const& a_stage_name, bool a_start_disabled = false);
        virtual ~MainGameStage() = default;

        virtual void OnUpdate() override;
        virtual void OnRenderImGui() override;

        virtual void OnAttached() override;
        virtual void OnRemoved() override;

    private:
        DFW::UniquePtr<DFW::DECS::ECSModule> _ecs_module;

        DFW::Entity _camera_entity;
        DFW::Entity _level;
        DFW::Entity _player;
        DFW::Entity _xyz;

        DFW::SharedPtr<DFW::FPSCharacterController> _player_controller;

    };

} // End of namespace ~ DGame.
