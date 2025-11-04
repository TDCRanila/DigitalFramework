#include <DFW/GameWorld/Controller/ControllerSystem.h>

namespace DFW
{
    void ControllerSystem::Update(DECS::EntityRegistry& a_registry)
    {
        for (auto&& [name, controller] : _controllers)
        {
            controller->ExecuteActions();
            controller->EraseActionList();
        }
    }

} // End of namespace ~ DFW.
