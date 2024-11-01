#include <DFW/GameWorld/Controller/BaseController.h>

namespace DFW
{
    void BaseController::ExecuteActions()
    {
        for (UniquePtr<Command>& command : _command_list)
        {
            command->Execute();
        }
    }

    void BaseController::EraseActionList()
    {
        for (UniquePtr<Command>& command : _command_list)
        {
            command.reset();
        }

        _command_list.clear();
    }

    void BaseController::QueueAction(UniquePtr<Command> a_command)
    {
        DFW_ASSERT(a_command);
        _command_list.emplace_back(std::move(a_command));
    }

} // End of namespace ~ DFW.
