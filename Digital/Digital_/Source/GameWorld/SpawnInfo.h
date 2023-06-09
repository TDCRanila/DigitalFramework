#pragma once

#include <GameWorld/Transform.h>
#include <Modules/ECS/Entity.h>

namespace DFW
{
    constexpr const char* DFW_DEFAULT_GAME_OBJECT_NAME = "Default GameObject Name";

    struct SpawnInfo
    {
        SpawnInfo();

        std::string name;
        Transform transform;
        Entity parent;

    };

} // End of namespace ~ DFW.
