#include <Modules/ECS/Managers/ECSComponentManager.h>

#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
    namespace DECS
    {
        ComponentManager::ComponentManager()
        {
            _keylock_system.GenerateComponentKeys();
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
