#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Utility/ECSCompBitList.h>

#include <string>

namespace DFW
{
    namespace DECS
    {
        struct EntityRegistrationComponent final : public DECS::Component::StrictRegistrar<EntityRegistrationComponent>
        {
            EntityRegistrationComponent()
                : name("")
                , id(DFW_INVALID_DUID)
                , comp_list(0)
            {
            }

            EntityRegistrationComponent(DFW::DUID a_id, std::string a_name)
                : name(a_name)
                , id(a_id)
                , comp_list(0)
            {
            }

            ~EntityRegistrationComponent() = default;

            std::string         name;
            DFW::DUID         id;
            ComponentBitList    comp_list;
        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
