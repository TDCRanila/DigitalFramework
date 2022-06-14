#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Utility/ECSCompBitList.h>

#include <string>

namespace DFW
{
    namespace DECS
    {
        constexpr const char* DFW_DEFAULT_ENTITY_NAME = "Default Entity Name";

        struct EntityRegistrationComponent final : public DECS::Component::StrictRegistrar<EntityRegistrationComponent>
        {
            EntityRegistrationComponent();
            EntityRegistrationComponent(DFW::DUID const a_id, std::string const& a_name);
            ~EntityRegistrationComponent() = default;

            DFW::DUID id;
            ComponentBitList comp_list;
            std::string name;
        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
