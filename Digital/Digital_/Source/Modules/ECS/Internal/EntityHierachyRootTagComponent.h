#pragma once

#include <Modules/ECS/Component.h>

namespace DFW
{
    namespace DECS
    {
        struct EntityHierachyRootTagComponent : public DECS::Component::StrictRegistrar<EntityHierachyRootTagComponent> 
        {
            EntityHierachyRootTagComponent() = default;
            virtual ~EntityHierachyRootTagComponent() = default;
        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
