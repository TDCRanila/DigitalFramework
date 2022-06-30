#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Utility/ECSCompBitList.h>
#include <Modules/ECS/Utility/ECSEntityType.h>

#include <string>

namespace DFW
{
    namespace DECS
    {
        constexpr const char* DFW_DEFAULT_ENTITY_NAME = "Default Entity Name";

        struct EntityDataComponent final : public DECS::Component::StrictRegistrar<EntityDataComponent>
        {
            EntityDataComponent();
            EntityDataComponent(DFW::DUID const a_id, EntityTypeID const a_type, std::string const& a_name);
            ~EntityDataComponent() = default;

            DFW::DUID id;
            ComponentBitList comp_list;
            EntityTypeID type;
            std::string name;
        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
