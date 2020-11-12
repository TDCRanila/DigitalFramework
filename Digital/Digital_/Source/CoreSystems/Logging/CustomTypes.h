#pragma once

#include <spdlog/fmt/ostr.h>

#include <Defines/IDDefines.h>

#pragma region IDs

    template<typename OStream>
    OStream& operator<<(OStream& os, const DCore::DUID& c)
    {
        return os << "DUID-" << c.operator std::string();
    }

#pragma endregion
