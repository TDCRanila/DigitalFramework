#include <GameWorld/Controller/CharacterController.h>

namespace DFW
{
    void CharacterController::ExecuteActions()
    { 
        for (Command* command : command_list) 
        { 
            command->Execute(); 
        } 
    }

    void CharacterController::EraseActionList()
    { 
        for (Command* command : command_list)
        {
            delete command;
        }

        command_list.clear(); 
    }

    void CharacterController::QueueAction(Command* a_command)
    {
        DFW_ASSERT(_controlled_character); 
        command_list.emplace_back(a_command);
    }

} // End of namespace ~ DFW.
