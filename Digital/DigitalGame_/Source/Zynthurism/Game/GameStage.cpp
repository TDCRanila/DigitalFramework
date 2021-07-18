#include <Zynthurism/Game/GameStage.h>

#include <CoreSystems/Logging/Logger.h>

#include <CoreSystems/Stage/StageEvents.h>

namespace DGame
{
    GameTestStage::GameTestStage(std::string a_stage_name, bool a_start_disabled)
        : DGame::GameStageBase(a_stage_name, a_start_disabled)
    {
    }

    void GameTestStage::RenderImGui()
    {
    }

    void GameTestStage::Update()
    {
        ecs.UpdateECS();
    }

    void GameTestStage::OnAttached()
    {
        ecs.InitECS();
    }

    void GameTestStage::OnRemoved()
    {
        ecs.TerminateECS();
    }

} // End of namespace ~ DGame.