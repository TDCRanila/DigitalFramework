#pragma once

#include <DFW/GameWorld/SpawnInfo.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/ECSModule.h>

#include <DFW/Utility/FilePath.h>

namespace DGame
{
    DFW::Entity SpawnLevel(DFW::FilePath const& a_map_file, DFW::SpawnInfo const& a_spawn_info, DFW::DECS::ECSModule& a_ecs);

} // End of namespace ~ DGame.
