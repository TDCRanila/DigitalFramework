#pragma once

// FW DEclare.
namespace DCore
{
    class GameClock;
} // End of namespace ~ DCore.

namespace DECS
{
    class ECSModule;
} // End of namespace ~ DECS.

class CoreService
{
public:

    static DCore::GameClock* GetGameClock();
    static void ProvideGameClock(DCore::GameClock* a_provided_service);

    static DECS::ECSModule* GetECS();
    static void ProvideECS(DECS::ECSModule* a_provided_service);

private:
    static DCore::GameClock* _gc_service;
    static DECS::ECSModule* _ecs_service;
};
    