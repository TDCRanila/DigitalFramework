#include <Modules/ECS/Managers/EntityRegistry.h>

#include <Modules/ECS/Entity.h>
#include <Modules/ECS/Internal/EntityDataComponent.h>

namespace DFW
{
    namespace DECS
    {
        EntityRegistry::EntityRegistry()
            : _id(DFW::GenerateDUID())
        {
            _entt_registry.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);

            _entity_duid_register.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
            _entity_name_register.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);

            _marked_entities_for_destruction.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
        }

        EntityRegistry::~EntityRegistry()
        {
            _entt_registry.clear();
            
            _marked_entities_for_destruction.clear();

            _entity_duid_register.clear();
            _entity_name_register.clear();
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
