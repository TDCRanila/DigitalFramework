#pragma once

#include <DFW/GameWorld/Controller/ControllerNameID.h>

#include <DFW/CoreSystems/Command.h>
#include <DFW/CoreSystems/Memory.h>

#include <vector>

namespace DFW
{
    class BaseController
    {
    friend class ControllerSystem;

    public:
        BaseController() = default;
        virtual ~BaseController() = default;

        ControllerNameID const& GetNameID() const { return _name_id; }

    protected:
        void ExecuteActions();
        void EraseActionList();
        void QueueAction(UniquePtr<Command> a_command);

    private:
        std::vector<UniquePtr<Command>> _command_list;
        ControllerNameID _name_id;
    };

} // End of namespace ~ DFW.
