#pragma once

#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Utility/ECSEntityType.h>

namespace DFW
{
    class GameObjectTagComponent : public DECS::Component::StrictRegistrar<GameObjectTagComponent>
    {
    public:
        GameObjectTagComponent() = default;
        ~GameObjectTagComponent() = default;

    };

} // End of namespace ~ DFW.
