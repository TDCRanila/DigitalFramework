#include <CoreSystems/CoreServices.h>

#include <Utility/TemplateUtility.h>

DCore::GameClock* CoreService::_gc_service = nullptr;
DECS::ECSModule* CoreService::_ecs_service = nullptr;

// TODO: Could provide some game clock null-service instead.
// maybe not needed for a gameclock but other services such as audio
// could be useful.

DCore::GameClock* CoreService::GetGameClock()
{
    DFW_ASSERT(_gc_service && "No Game Clock provided.");      
    return _gc_service;
}

void CoreService::ProvideGameClock(DCore::GameClock* a_provided_service)
{
    _gc_service = a_provided_service;
}

DECS::ECSModule* CoreService::GetECS()
{
    DFW_ASSERT(_ecs_service && "No DECS Module provided.");
    return _ecs_service;
}

void CoreService::ProvideECS(DECS::ECSModule* a_provided_service)
{
    _ecs_service = a_provided_service;
}
