#pragma once

#include <GameWorld/SpawnInfo.h>

#include <Modules/ECS/Entity.h>
#include <Modules/ECS/ECSModule.h>

#include <Utility/FilePath.h>

namespace DGame
{
    DFW::Entity SpawnLevel(DFW::FilePath const& a_map_file, DFW::SpawnInfo const& a_spawn_info, DFW::DECS::ECSModule& a_ecs);

} // End of namespace ~ DGame.
