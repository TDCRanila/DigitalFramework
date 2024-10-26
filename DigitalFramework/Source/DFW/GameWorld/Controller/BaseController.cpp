#include <DFW/GameWorld/Controller/BaseController.h>

namespace DFW
{
    void BaseController::ExecuteActions()
    {
        for (Command* command : command_list)
        {
            command->Execute();
        }
    }

    void BaseController::EraseActionList()
    {
        for (Command* command : command_list)
        {
            delete command;
        }

        command_list.clear();
    }

    void BaseController::QueueAction(Command* a_command)
    {
        command_list.emplace_back(a_command);
    }

} // End of namespace ~ DFW.
