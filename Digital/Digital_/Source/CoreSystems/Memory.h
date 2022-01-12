#pragma once

#include <memory>

namespace DFW
{
    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T>
    using UniquePtr = std::unique_ptr<T>;

    template <typename T>
    using WeakPtr = std::weak_ptr<T>;

    template <typename T, typename... TArgs>
    constexpr SharedPtr<T> MakeShared(TArgs&& ... a_args)
    {
        return std::make_shared<T>(std::forward<TArgs>(a_args)...);
    }

    template <typename T, typename... TArgs>
    constexpr UniquePtr<T> MakeUnique(TArgs&& ... a_args)
    {
        return std::make_unique<T>(std::forward<TArgs>(a_args)...);
    }

} // End of namespace ~ DFW.
