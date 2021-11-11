#pragma once

namespace DFW
{
    namespace DECS
    {
        // FW Declare.
        class System;
        class Component;

        template <typename SystemType>
        concept IsValidSystemType = IsDerivedFrom<SystemType, System>;

        template <typename ComponentType>
        concept IsValidComponentType = IsDerivedFrom<ComponentType, Component>;

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
