#include <GameWorld/TransformSystem.h>

#include <GameWorld/TransformComponent.h>

#include <Modules/ECS/Entity.h>
#include <Modules/ECS/Internal/EntityRelationComponent.h>
#include <Modules/ECS/Internal/EntityHierachyRootTagComponent.h>

namespace DFW
{
    namespace Detail
    {
        void CalculateEntityTransform(Entity& a_current_entity, Entity const& a_parent, bool a_is_parent_dirty)
        {
            // Calculate the world transform if necesarry.
            TransformComponent& transform_component = a_current_entity.GetComponent<TransformComponent>();
            bool const is_transform_dirty = transform_component.IsDirty() || a_is_parent_dirty;
            if (is_transform_dirty)
                transform_component.CalculateWorldTransform(a_parent.GetComponent<TransformComponent>().GetWorldTransformMatrix());

            DECS::EntityRelationComponent const* relation_component = a_current_entity.TryGetComponent<DECS::EntityRelationComponent>();
            if (!relation_component)
                return; // Nothing else to calculate, no childeren or siblings.

            // Traverse entity hierachy.
            Entity current_child = relation_component->first;
            while (current_child.IsEntityValid())
            {
                CalculateEntityTransform(current_child, a_current_entity, is_transform_dirty);
                if (DECS::EntityRelationComponent const* child_relation_component = current_child.TryGetComponent<DECS::EntityRelationComponent>())
                    current_child = child_relation_component->next;
            }
        }

        void CalculateEntityTransformFromRoot(Entity& a_current_entity, Entity const& a_parent, bool a_is_parent_dirty)
        {
            // Calculate the world transform if necesarry.
            TransformComponent& transform_component = a_current_entity.GetComponent<TransformComponent>();
            bool const is_transform_dirty = transform_component.IsDirty() || a_is_parent_dirty;
            if (is_transform_dirty)
                transform_component.CalculateWorldTransform();

            DECS::EntityRelationComponent const* relation_component = a_current_entity.TryGetComponent<DECS::EntityRelationComponent>();
            if (!relation_component)
                return; // Nothing else to calculate, no childeren or siblings.

            // Traverse entity hierachy.
            Entity current_child = relation_component->first;
            while (current_child.IsEntityValid())
            {
                CalculateEntityTransform(current_child, a_current_entity, is_transform_dirty);
                if (DECS::EntityRelationComponent* child_relation_component = current_child.TryGetComponent<DECS::EntityRelationComponent>())
                {
                    current_child = child_relation_component->next;
                }
            }
        }

    } // End of namespace ~ Detail

    void TransformSystem::PreUpdate(DECS::EntityRegistry& a_registry)
    {
        // TODO: Should be done in Init, need to change System's Init & Terminate to passthrough
        // the registry as well.
        a_registry.GetHierachyRoot().AddComponent<DFW::TransformComponent>();
    }

    void TransformSystem::Update(DECS::EntityRegistry& a_registry)
    {
        // TODO:
        // Should look into using DirtyTransformTagComponents in combination with 
        // sorting of entities, instead of using boolean dirty flags and traversing the entity hierachy.
        // This way we don't have to traverse entity trees that do not need a transform update.
        // For now this implementation works more than fine.
        Entity root_entity(a_registry.ENTT().view<DECS::EntityHierachyRootTagComponent>().front(), a_registry);
        Detail::CalculateEntityTransformFromRoot(root_entity, Entity(), false);

    }


} // End of namespace ~ DFW.
