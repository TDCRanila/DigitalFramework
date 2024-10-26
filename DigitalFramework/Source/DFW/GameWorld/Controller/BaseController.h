#pragma once

#include <DFW/CoreSystems/Command.h>
#include <DFW/CoreSystems/Memory.h>

#include <vector>

namespace DFW
{
    class BaseController
    {
    public:
        BaseController() = default;
        virtual ~BaseController() = default;

        void ExecuteActions();
        void EraseActionList();
        void QueueAction(Command* a_command);

    private:
        std::vector<Command*> command_list;

    };

} // End of namespace ~ DFW.
