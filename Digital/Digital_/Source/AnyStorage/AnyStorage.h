#pragma once

#include <vector>

namespace DAny 
{
    template <typename T>
    using AnyStorage = std::vector<T*>;

} // End of namespace ~ Any
