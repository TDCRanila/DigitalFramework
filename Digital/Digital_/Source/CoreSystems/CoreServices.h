#pragma once

namespace DFW
{
    // FW Declare.
    class GameClock;

    namespace DECS
    {
        class ECSModule;
    } // End of namespace ~ DECS.

    class CoreService
    {
    public:

        static GameClock* GetGameClock();
        static void ProvideGameClock(GameClock* a_provided_service);

        static DECS::ECSModule* GetECS();
        static void ProvideECS(DECS::ECSModule* a_provided_service);

    private:
        static GameClock* _gc_service;
        static DECS::ECSModule* _ecs_service;
    };

} // End of namespace ~ DFW.
