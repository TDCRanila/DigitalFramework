#pragma once

#include <DFW/Defines/MathDefines.h>

#include <DFW/Utility/FilePath.h>

#include <boost/container_hash/hash.hpp>

#include <entt/resource/resource.hpp>
#include <entt/core/hashed_string.hpp>

namespace DFW
{
    template <typename ResourceType>
    using ResourceHandle = entt::resource<ResourceType>;

    constexpr entt::id_type DFW_INVALID_RESOUCRE_ID = DMath::GetMinValueOfType<entt::id_type>();

    struct Resource
    {
        Resource() = default;

        FilePath filepath;
    };

} // End of namespace ~ DFW.
