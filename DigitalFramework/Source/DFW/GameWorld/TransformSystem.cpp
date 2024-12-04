#include <DFW/GameWorld/TransformSystem.h>

#include <DFW/GameWorld/TransformComponent.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Internal/EntityRelationComponent.h>
#include <DFW/Modules/ECS/Internal/EntityHierachyRootTagComponent.h>

namespace DFW
{
    namespace Detail
    {
        void CalculateEntityTransform(Entity& a_current_entity, glm::mat4 const& a_parent_transform, bool const a_is_parent_dirty)
        {
            bool is_transform_dirty(true);
            glm::mat4 current_transform(glm::identity<glm::mat4>());
            if (TransformComponent* transform_component = a_current_entity.TryGetComponent<TransformComponent>())
            {
                is_transform_dirty = transform_component->IsDirty() || a_is_parent_dirty;
                if (is_transform_dirty)
                {
                    transform_component->CalculateWorldTransform(a_parent_transform);
                    current_transform = transform_component->GetWorldTransformMatrix();
                }
            }

            DECS::EntityRelationComponent* relation_component = a_current_entity.TryGetComponent<DECS::EntityRelationComponent>();
            if (!relation_component)
                return; // Nothing else to calculate, no childeren or siblings.

            // Traverse entity hierachy.
            Entity* current_child = &relation_component->first;
            while (current_child->IsEntityValid())
            {
                CalculateEntityTransform(*current_child, current_transform, is_transform_dirty);
                current_child = &current_child->GetComponent<DECS::EntityRelationComponent>().next;
            }
        }

        void CalculateEntityTransformFromRoot(Entity& a_root_entity)
        {
            // Calculate the world transform if necesarry.
            TransformComponent& transform_component = a_root_entity.GetComponent<TransformComponent>();
            if (transform_component.IsDirty())
                transform_component.CalculateWorldTransform(glm::identity<glm::mat4>());

            DECS::EntityRelationComponent* relation_component = a_root_entity.TryGetComponent<DECS::EntityRelationComponent>();
            if (!relation_component)
                return; // Nothing else to calculate, no childeren or siblings.

            // Traverse entity hierachy.
            Entity* current_child = &relation_component->first;
            while (current_child->IsEntityValid())
            {
                CalculateEntityTransform(*current_child, transform_component.GetWorldTransformMatrix(), transform_component.IsDirty());
                current_child = &current_child->GetComponent<DECS::EntityRelationComponent>().next;
            }
        }

    } // End of namespace ~ Detail

    void TransformSystem::Init(DECS::EntityRegistry& a_registry)
    {
        a_registry.GetHierachyRoot().AddComponent<DFW::TransformComponent>();
    }

    void TransformSystem::Terminate(DECS::EntityRegistry& a_registry)
    {
        a_registry.GetHierachyRoot().DeleteComponent<DFW::TransformComponent>();
    }

    void TransformSystem::Update(DECS::EntityRegistry& a_registry)
    {
        // TODO:
        // Should look into using DirtyTransformTagComponents in combination with 
        // sorting of entities, instead of using boolean dirty flags and traversing the entity hierachy.
        // This way we don't have to traverse entity trees that do not need a transform update.
        // For now this implementation works more than fine.
        // As well using a children hierarchy depth to traverse entities linearly.
        Entity root_entity(a_registry.ENTT().view<DECS::EntityHierachyRootTagComponent>().front(), a_registry);
        Detail::CalculateEntityTransformFromRoot(root_entity);

    }


} // End of namespace ~ DFW.
