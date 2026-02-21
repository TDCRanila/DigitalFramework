#pragma once

#include <DFW/GameWorld/Transform.h>

#include <DFW/Modules/ECS/Entity.h>

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
