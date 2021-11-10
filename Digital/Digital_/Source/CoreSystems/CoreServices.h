#pragma once

// FW DEclare.
namespace DCore
{
    class GameClock;
} // End of namespace ~ DCore.

namespace ECS
{
    class ECSModule;
} // End of namespace ~ ECS.

class CoreService
{
public:

    static DCore::GameClock* GetGameClock();
    static void ProvideGameClock(DCore::GameClock* a_provided_service);

    static ECS::ECSModule* GetECS();
    static void ProvideECS(ECS::ECSModule* a_provided_service);

private:
    static DCore::GameClock* _gc_service;
    static ECS::ECSModule* _ecs_service;
};
    