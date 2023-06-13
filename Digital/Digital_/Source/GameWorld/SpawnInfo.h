#pragma once

#include <GameWorld/Transform.h>

#include <Modules/ECS/Entity.h>

namespace DFW
{
    struct SpawnInfo
    {
        SpawnInfo() = default;

        std::string name;
        Transform transform;
        Entity parent;

    };

} // End of namespace ~ DFW.
