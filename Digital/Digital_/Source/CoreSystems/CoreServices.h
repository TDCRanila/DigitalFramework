#pragma once

// FW DEclare.
namespace DCore
{
    class GameClock;
} // End of namespace ~ DCore.


class CoreService
{
public:

    static DCore::GameClock* GetGameClock();
    static void ProvideGameClock(DCore::GameClock* a_provided_service);

private:
    static DCore::GameClock* _gc_service;
};
    