#pragma once

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>

#pragma region CoreTypes

#include <CoreSystems/DUID.h>

    template <> struct fmt::formatter<DFW::DUID> : fmt::formatter<std::string>
    {
        auto format(DFW::DUID a_duid, format_context& ctx) const
        {
            return formatter<std::string>::format(a_duid.operator std::string(), ctx);
        }
    };

#pragma endregion

#pragma region GLM

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <Modules/Math/EulerAngles.h>

template <> struct fmt::formatter<glm::vec2>
{
    char presentation = 'f';

    constexpr format_parse_context::iterator parse(format_parse_context& ctx)
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;
        if (it != end && *it != '}') ctx.on_error("invalid format");

        return it;
    }

    format_context::iterator format(glm::vec3 const& a_vec2, format_context& ctx)
    {
        return presentation == 'f'
            ? fmt::format_to(ctx.out(), "({:.1f}, {:.1f})", a_vec2.x, a_vec2.y)
            : fmt::format_to(ctx.out(), "({:.1e}, {:.1e})", a_vec2.x, a_vec2.y);
    }
};

template <> struct fmt::formatter<glm::vec3>
{
    char presentation = 'f';

    constexpr format_parse_context::iterator parse(format_parse_context& ctx)
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;
        if (it != end && *it != '}') ctx.on_error("invalid format");

        return it;
    }

    format_context::iterator format(glm::vec3 const& a_vec3, format_context& ctx)
    {
        // ctx.out() is an output iterator to write to.
        return presentation == 'f'
            ? fmt::format_to(ctx.out(), "({:.1f}, {:.1f}, {:.1f})", a_vec3.x, a_vec3.y, a_vec3.z)
            : fmt::format_to(ctx.out(), "({:.1e}, {:.1e}, {:.1e})", a_vec3.x, a_vec3.y, a_vec3.z);
    }
};

template <> struct fmt::formatter<DFW::DMath::EulerAngles> : formatter<glm::vec3>
{
    format_context::iterator format(DFW::DMath::EulerAngles const& a_eulerangle, format_context& ctx)
    {
        return formatter<glm::vec3>::format(a_eulerangle, ctx);
    }
};

template <> struct fmt::formatter<glm::vec4>
{
    char presentation = 'f';

    constexpr format_parse_context::iterator parse(format_parse_context& ctx)
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;
        if (it != end && *it != '}') ctx.on_error("invalid format");

        return it;
    }

    format_context::iterator format(glm::vec4 const& a_vec4, format_context& ctx) const 
    {
        return presentation == 'f'
            ? fmt::format_to(ctx.out(), "({:.1f}, {:.1f}, {:.1f}, {:.1f})", a_vec4.x, a_vec4.y, a_vec4.z, a_vec4.w)
            : fmt::format_to(ctx.out(), "({:.1e}, {:.1e}, {:.1e}, {:.1e})", a_vec4.x, a_vec4.y, a_vec4.z, a_vec4.w);
    }
};

#pragma endregion
