#include <Zynthurism/Game/TestStage.h>

#include <Zynthurism/Game/GameplaySystems/TestSystem.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/Managers/ECSEntityManager.h>
#include <Modules/ECS/Managers/ECSystemManager.h>
#include <Modules/ECS/Objects/ECSUniverse.h>


namespace DGame
{
    TestStage::TestStage(std::string a_stage_name, bool a_start_disabled)
        : DGame::GameStageBase(a_stage_name, a_start_disabled)
    {
        ecs = CoreService::GetECS();
    }

    void TestStage::RenderImGui()
    {
    }

    void TestStage::Update()
    {
    }

    void TestStage::OnAttached()
    {
        _universe = ecs->RegisterUniverse("Main");

        for (int8 i = 0; i < 64; ++i)
        {
            auto entity = ecs->EntityManager()->CreateEntity(_universe);
            ecs->EntityManager()->AddComponent<TestComponent>(entity);
        }
    }

    void TestStage::OnRemoved()
    {
    }

} // End of namespace ~ DGame.