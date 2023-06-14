#include <Modules/ECS/Managers/EntityRegistry.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Entity.h>
#include <Modules/ECS/EntityEvents.h>
#include <Modules/ECS/Internal/EntityDataComponent.h>
#include <Modules/ECS/Internal/EntityRelationComponent.h>
#include <Modules/ECS/Internal/EntityHierachyRootTagComponent.h>

#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
    namespace DECS
    {
        EntityRegistry::EntityRegistry(ECSModule& a_ecs)
            : _id(DFW::GenerateDUID())
            , _ecs_event_handler(a_ecs.EventHandler())
        {
            _entt_registry.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);

            _entity_duid_register.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
            _entity_name_register.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);

            _marked_entities_for_destruction.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);

            _entity_hierachy_root = _entt_registry.create();
            _entt_registry.emplace<DECS::EntityHierachyRootTagComponent>(_entity_hierachy_root);
        }

        EntityRegistry::~EntityRegistry()
        {
            _entt_registry.clear();
            
            _marked_entities_for_destruction.clear();

            _entity_duid_register.clear();
            _entity_name_register.clear();
        }

        Entity EntityRegistry::CreateEntity()
        {
            return CreateEntity(GetHierachyRoot());
        }

        Entity EntityRegistry::CreateEntity(Entity const& a_parent_entity)
        {
            // Construct a new Entity.
            Entity entity(ENTT().create(), *this);

            // Setup additional Entity data.
            EntityDataComponent& data_component = entity.AddComponent<EntityDataComponent>();
            data_component.id   = DFW::GenerateDUID();
            data_component.type = GetEntityTypeID<"">();
            data_component.name = DFW_DEFAULT_ENTITY_NAME + std::to_string(static_cast<ENTT_ID_TYPE>(entity.GetHandle()));

            // Parent-Child relationship.
            DFW_ASSERT(a_parent_entity.IsEntityValid());
            entity.SetParent(a_parent_entity);

            // Register Entity in EntityRegistry registers.
            RegisterEntity(entity.GetHandle());

            // Broadcast Entity Creation.
            _ecs_event_handler.get().Broadcast<EntityCreatedEvent>(entity);

            return entity;
        }

        void EntityRegistry::DestroyEntity(Entity const& a_entity)
        {
            DFW_ASSERT(*this == a_entity.GetRegistry());

            if (!a_entity.IsEntityValid())
            {
                DFW_WARNLOG("Attempting to delete an entity that is invalid.");
                return;
            }

            DestroyEntityAndChilderen(a_entity);
        }

        Entity EntityRegistry::GetHierachyRoot()
        {
            return Entity(_entity_hierachy_root, *this);
        }

        Entity EntityRegistry::GetEntity(DFW::DUID const a_entity_id)
        {
            EntityDUIDRegisterMap const& registery = _entity_duid_register;
            if (auto const& it = registery.find(a_entity_id); it != registery.end())
                return Entity(it->second, *this);

            return Entity();
        }

        Entity EntityRegistry::GetEntity(std::string const& a_entity_name)
        {
            EntityNameRegisterMap const& registery = _entity_name_register;
            if (auto const& it = registery.find(a_entity_name); it != registery.end())
                return Entity(it->second, *this);

            return Entity();
        }

        std::vector<Entity> EntityRegistry::GetEntities()
        {           
            std::vector<Entity> entities;
            entities.resize(_entt_registry.size());

            EntityHandle const* entity_data = _entt_registry.data();
            for (uint32 entity_handle_index(0); entity_handle_index < _entt_registry.size(); entity_handle_index++)
            {
                entities.emplace_back(entity_data[entity_handle_index], *this);
            }
            return entities;
        }

        void EntityRegistry::RegisterEntity(EntityHandle const a_entity_handle)
        {
            DFW_ASSERT(_entt_registry.valid(a_entity_handle));

            // Store EntityData in quick-access containers.
            EntityDataComponent const& data_component = _entt_registry.get<EntityDataComponent>(a_entity_handle);
            _entity_duid_register.emplace(data_component.id, a_entity_handle);
            _entity_name_register.emplace(data_component.name, a_entity_handle);
        }

        void EntityRegistry::UnregisterEntity(EntityHandle const a_entity_handle)
        {
            DFW_ASSERT(_entt_registry.valid(a_entity_handle));

            // Remove EntityData in quick-access containers.
            EntityDataComponent const& data_component = _entt_registry.get<EntityDataComponent>(a_entity_handle);
            _entity_duid_register.erase(data_component.id);
            _entity_name_register.erase(data_component.name);
        }

        bool EntityRegistry::IsEntityMarkedForDestruction(EntityHandle const a_entity_handle) const
        {
            if (auto const& it = _marked_entities_for_destruction.find(a_entity_handle); it != _marked_entities_for_destruction.end())
                return true;
            else
                return false;
        }

        void EntityRegistry::DestroyEntityAndChilderen(Entity const& a_current_entity)
        {
            std::unordered_set<EntityHandle>& marked_entities = _marked_entities_for_destruction;
            auto const& [it, insertion_result] = marked_entities.emplace(a_current_entity.GetHandle());
            if (!insertion_result)
                DFW_WARNLOG("Attemping to delete an entity that is already marked for deletion.");

            // Broadcast Entity Destruction.
            _ecs_event_handler.get().Broadcast<EntityDestroyedEvent>(a_current_entity);

            DECS::EntityRelationComponent const* relation_component = a_current_entity.TryGetComponent<DECS::EntityRelationComponent>();
            if (!relation_component)
                return; // No childeren or siblings.

            // Traverse entity hierachy.
            Entity current_child = relation_component->first;
            while (current_child.IsEntityValid())
            {
                DestroyEntityAndChilderen(current_child);

                if (DECS::EntityRelationComponent const* child_relation_component = current_child.TryGetComponent<DECS::EntityRelationComponent>())
                    current_child = child_relation_component->next;
            }
        }

        void EntityRegistry::CleanDestructionMarkedEntities()
        {
            std::unordered_set<EntityHandle>& marked_entities = _marked_entities_for_destruction;
            if (marked_entities.empty())
                return;

            for (EntityHandle const handle : marked_entities)
            {
                UnregisterEntity(handle);
            }

            _entt_registry.destroy(marked_entities.begin(), marked_entities.end());

            marked_entities.clear();
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
