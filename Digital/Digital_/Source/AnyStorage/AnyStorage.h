#pragma once

#include <vector>

namespace Any 
{
    template <typename T>
    using AnyStorage = std::vector<T*>;

} // End of namespace ~ Any
