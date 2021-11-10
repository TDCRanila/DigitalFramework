#pragma once

namespace ECS
{
    // FW Declare.
    class System;
    class Component;

    template <typename SystemType>
    concept IsValidSystemType = IsDerivedFrom<SystemType, System>;

    template <typename ComponentType>
    concept IsValidComponentType = IsDerivedFrom<ComponentType, Component>;

} // End of namespace ~ ECS.
