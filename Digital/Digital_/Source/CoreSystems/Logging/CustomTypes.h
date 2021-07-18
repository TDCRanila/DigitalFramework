#pragma once

#include <spdlog/fmt/ostr.h>

#include <CoreSystems/DObject.h>

#include <Defines/IDDefines.h>

#pragma region CoreTypes

    // TODO Need to look into this again regarding operators for custom logging types.

    //template<typename OStream>
    //OStream& operator<<(OStream& os, const DCore::DUID& c)
    //{
    //    return os << "DUID-" << c.operator std::string();
    //}

    //template<typename OStream>
    //OStream& operator<<(OStream& os, const DCore::DObject& c)
    //{
    //    return os << "DObject-" << c.operator std::string();
    //}

#pragma endregion
