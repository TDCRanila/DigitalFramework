#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Utility/ECSCompBitList.h>

#include <string>

namespace DECS
{
    struct ECSEntityRegistrationComponent final : public DECS::ECSComponent::StrictRegistrar<ECSEntityRegistrationComponent>
    {
        ECSEntityRegistrationComponent()
            : name("")
            , id(DCore::DFW_INVALID_DUID)
            , comp_list(0)
        {
        }

        ECSEntityRegistrationComponent(DCore::DUID a_id, std::string a_name) 
            : name(a_name)
            , id (a_id)
            , comp_list(0)
        {
        }

        ~ECSEntityRegistrationComponent() = default;

        std::string         name;
        DCore::DUID         id;
        ComponentBitList    comp_list;
    };

} // End of namespace ~ DECS.
