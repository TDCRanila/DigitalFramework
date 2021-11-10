#pragma once

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/GameClock.h>

#include <CoreSystems/TimeTracker.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Objects/ECSUniverse.h>
#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Managers/ECSEntityManager.h>
#include <Modules/ECS/Managers/ECSEventHandler.h>

#include <ranges>

namespace DGame
{
    class TestComponent final : public ECS::Component::StrictRegistrar<TestComponent>
    {
    public:
        TestComponent() = default;

        float32 value;
    };

    struct TestEvent
    {
        TestEvent() = default;
        TestEvent(ECS::Entity a_entity, float32 a_old_value, float32 a_new_value)
        {
            entity      = a_entity;
            old_value   = a_old_value;
            new_value   = a_new_value;
        }

        ECS::Entity entity;
        float32 old_value;
        float32 new_value;
    };

    inline void EventTestFunction(TestEvent& a_event)
    {
        DFW_INFOLOG("Testing EventTestFunction");
    }

    class TestSystem : public ECS::System::StrictRegistrar<TestSystem>
    {
    public:
        TestSystem() = default;
        ~TestSystem() = default;

        void Init() override
        {
            CoreService::GetECS()->EventHandler()->RegisterCallback<TestEvent, &TestSystem::OnValueAddition>(this);
            CoreService::GetECS()->EventHandler()->RegisterCallback<TestEvent, &DGame::EventTestFunction>();
        }

        void Terminate() override
        {
            CoreService::GetECS()->EventHandler()->UnregisterCallback<TestEvent, &TestSystem::OnValueAddition>(this);
            CoreService::GetECS()->EventHandler()->UnregisterCallback<TestEvent, &DGame::EventTestFunction>();
        }

        void OnValueAddition(TestEvent& a_event)
        {
            DFW_INFOLOG("Value Addition Event Received: {}, {}, {}", a_event.entity.GetID(), a_event.new_value, a_event.old_value);
        }

        inline virtual void Update(ECS::Universe* const a_universe) override
        {
            float32 dt = CoreService::GetGameClock()->GetLastFrameDeltaTime();
            
            static DCore::TimeTracker timer(true);

            auto view = a_universe->_registry.view<TestComponent>();

            for (auto& entity : view)
            {
                if (timer.FetchElapsedTime() > DCore::TimeUnit(15.0f))
                {
                    TestComponent& comp1  = a_universe->_registry.get<TestComponent>(entity);

                    float32 old_value = comp1.value;
                    float32 new_value = comp1.value + dt;
                    TestEvent val_event(comp1.GetOwner(), old_value, new_value);

                    CoreService::GetECS()->EventHandler()->Broadcast(val_event);
                    CoreService::GetECS()->EventHandler()->Broadcast<TestEvent>();
                    timer.ResetTimer(true);
                }
            }
        }

    private:

    };

} // End of namespace ~ DGame.
