#pragma once

#include <vector>

namespace DFW
{
    namespace DAny
    {
        template <typename T>
        using AnyStorage = std::vector<T*>;

    } // End of namespace ~ DAny.

} // End of namespace ~ DFW.
