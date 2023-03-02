#pragma once


#include <Modules/ECS/Component.h>
#include <Modules/ECS/Utility/EntityTypeID.h>

#include <CoreSystems/DUID.h>

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
            EntityTypeID type;
            std::string name;
        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
