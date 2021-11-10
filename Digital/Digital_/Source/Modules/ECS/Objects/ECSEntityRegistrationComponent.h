#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Utility/ECSCompBitList.h>

#include <string>

namespace ECS
{
    struct EntityRegistrationComponent final : public ECS::Component::StrictRegistrar<EntityRegistrationComponent>
    {
        EntityRegistrationComponent()
            : name("")
            , id(DCore::DFW_INVALID_DUID)
            , comp_list(0)
        {
        }

        EntityRegistrationComponent(DCore::DUID a_id, std::string a_name) 
            : name(a_name)
            , id (a_id)
            , comp_list(0)
        {
        }

        ~EntityRegistrationComponent() = default;

        std::string         name;
        DCore::DUID         id;
        ComponentBitList    comp_list;
    };

} // End of namespace ~ ECS.
