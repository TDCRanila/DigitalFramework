#pragma once

#include <GameWorld/Transform.h>

namespace DFW
{
    constexpr const char* DFW_DEFAULT_GAME_OBJECT_NAME = "Default GameObject Name";

    struct SpawnInfo
    {
        SpawnInfo();

        std::string name;
        Transform transform;

    };

} // End of namespace ~ DFW.
