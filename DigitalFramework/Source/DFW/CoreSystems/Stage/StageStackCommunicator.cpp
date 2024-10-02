#include <DFW/CoreSystems/Stage/StageStackCommunicator.h>

#include <DFW/CoreSystems/Stage/StageStackController.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace DFW
{
    void StageStackCommunicator::OnStageEventReceived(StageEvent& a_event)
    {
        // TODO DEBUG TAG / IF ELSE
        if (a_event.GetDebugString().empty())
            DFW_INFOLOG("StageEvent Received: Name; {}", a_event.GetName());
        else
            DFW_INFOLOG("StageEvent Received: Name; {} - Debug; {}", a_event.GetName(), a_event.GetDebugString());

        for (auto stage_it = _stage_stack_controller->rbegin(); stage_it != _stage_stack_controller->rend(); ++stage_it)
        {
            StageBase* stage_ptr = (*stage_it);
            //if (!stage_ptr->IsDisabled())
            //    stage_ptr->OnStageEvent(a_event);
        }
    }

    void StageStackCommunicator::SetStageStackController(const StageStackController& a_controller)
    {
        _stage_stack_controller = &a_controller;
    }

} // End of namespace ~ DFW.
