#include <CoreSystems/CoreServices.h>

#include <Utility/TemplateUtility.h>

DCore::GameClock* CoreService::_gc_service = nullptr;

DCore::GameClock* CoreService::GetGameClock()
{
    // TODO: Should provide some game clock null service instead.
    if (_gc_service == nullptr)
        DFW_ASSERT(true, "No GameClock Provided.");
        
    return _gc_service;
}

void CoreService::ProvideGameClock(DCore::GameClock* a_provided_service)
{
    _gc_service = a_provided_service;
}
