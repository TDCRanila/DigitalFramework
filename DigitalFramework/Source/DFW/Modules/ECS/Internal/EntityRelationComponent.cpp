#include <DFW/Modules/ECS/Internal/EntityRelationComponent.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace DFW
{
    namespace DECS
    {
        void Debug_LogAndTraverseEntityHierachy(Entity const& a_entity)
        {
            DECS::EntityRelationComponent const* relation_component = a_entity.TryGetComponent<DECS::EntityRelationComponent>();
            if (!relation_component)
                return; // No childeren or siblings.

            std::string begin_string(std::to_string(relation_component->hierarchy_depth));
            std::string end_string(std::to_string(relation_component->hierarchy_depth));
            for (int32 index(0); index < relation_component->hierarchy_depth + 1; index++)
            {
                begin_string.append(">");
                end_string.append("<");
            }

            // Traverse entity hierachy.
            DFW_INFOLOG("{} [{} ({})]", begin_string, a_entity.GetName(), std::to_string(static_cast<int32>(a_entity.GetHandle())));
            
            Entity const* current_child = &relation_component->first;
            while (current_child->IsEntityValid())
            {
                Debug_LogAndTraverseEntityHierachy(*current_child);
                current_child = &current_child->GetComponent<DECS::EntityRelationComponent>().next;
            }

            DFW_INFOLOG("{} [{} ({})]", end_string, a_entity.GetName(), std::to_string(static_cast<int32>(a_entity.GetHandle())));
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
