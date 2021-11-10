#pragma once

namespace DECS
{
    // FW Declare.
    class ECSystem;
    class ECSComponent;

    template <typename SystemType>
    concept IsValidSystemType = IsDerivedFrom<SystemType, ECSystem>;

    template <typename ComponentType>
    concept IsValidComponentType = IsDerivedFrom<ComponentType, ECSComponent>;

} // End of namespace ~ DECS.
