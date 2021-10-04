#include <CoreSystems/CoreServices.h>

#include <Utility/TemplateUtility.h>

DCore::GameClock* CoreService::_gc_service = nullptr;

DCore::GameClock* CoreService::GetGameClock()
{
    // TODO: Could provide some game clock null-service instead.
    // maybe not needed for a gameclock but other services such as audio
    // could be useful.
    DFW_ASSERT(_gc_service && "No Game Clock provided.");      
    return _gc_service;
}

void CoreService::ProvideGameClock(DCore::GameClock* a_provided_service)
{
    _gc_service = a_provided_service;
}
