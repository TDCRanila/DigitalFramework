#include <Modules/ECS/Managers/ECSComponentManager.h>

#include <CoreSystems/Logging/Logger.h>

namespace ECS 
{
    ComponentManager::ComponentManager()
    {
        _keylock_system.GenerateComponentKeys();
    }

} // End of namespace ~ ECS
