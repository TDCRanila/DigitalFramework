#pragma once

#include <DFW/Modules/ECS/Component.h>
#include <DFW/Modules/ECS/Entity.h>

#include <DFW/Defines/MathDefines.h>

namespace DFW
{
    namespace DECS
    {
        void Debug_LogAndTraverseEntityHierachy(Entity const& a_entity);
        
        struct EntityRelationComponent : public DECS::Component::StrictRegistrar<EntityRelationComponent>
        {
            EntityRelationComponent() : childeren_count(0), hierarchy_depth(DFW::DMath::uint16_max) {}

            Entity parent; // Parent of entity that owns this component.
            
            uint16 childeren_count;
            uint16 hierarchy_depth;
            Entity first; // First child of the parent.
            Entity next; // Next sibling / Next child for the parent.
            Entity previous; // Previous sibling / Previous child for the parent.

        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
