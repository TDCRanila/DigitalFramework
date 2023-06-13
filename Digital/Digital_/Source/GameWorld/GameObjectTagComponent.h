#pragma once

#include <Modules/ECS/Component.h>

namespace DFW
{
    class GameObjectTagComponent : public DECS::Component::StrictRegistrar<GameObjectTagComponent>
    {
    public:
        GameObjectTagComponent() = default;
        ~GameObjectTagComponent() = default;

    };

} // End of namespace ~ DFW.
