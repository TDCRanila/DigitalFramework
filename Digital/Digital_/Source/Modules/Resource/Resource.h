#pragma once

#include <Defines/MathDefines.h>

#include <boost/container_hash/hash.hpp>

#include <entt/resource/resource.hpp>
#include <entt/core/hashed_string.hpp>

namespace DFW
{
    template <typename ResourceType>
    using ResourceHandle = entt::resource<ResourceType>;

    constexpr entt::id_type DFW_INVALID_RESOUCRE_ID = DMath::GetMinValueOfType<entt::id_type>();

} // End of namespace ~ DFW.
