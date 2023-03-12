#include <Modules/ECS/Managers/EntityRegistry.h>

#include <Modules/ECS/Entity.h>
#include <Modules/ECS/Internal/EntityDataComponent.h>

namespace DFW
{
    namespace DECS
    {
        EntityRegistry::EntityRegistry(std::string const& a_registry_name)
            : _id(DFW::GenerateDUID())
            , _name(a_registry_name)
        {
            _entt_registry.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);

            _entity_duid_register.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
            _entity_name_register.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);

            _registered_entities.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
            _marked_entities_for_destruction.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
        }

        EntityRegistry::~EntityRegistry()
        {
            _entt_registry.clear();
            
            _registered_entities.clear();
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
            entities.resize(_registered_entities.size());
            
            // Create Entities from Entity Handles.
            for (auto const& entity_handle : _registered_entities)
            {
                entities.emplace_back(entity_handle, *this);
            }

            return entities;
        }

        bool EntityRegistry::IsValid() const
        {
            return _id != DFW_INVALID_DUID;
        }

        void EntityRegistry::RegisterEntity(InternalEntity const& a_entity)
        {
            DFW_ASSERT(a_entity.IsEntityValid());

            // Store Entity Handle.
            EntityHandle const entity_handle = a_entity.GetHandle();
            _registered_entities.emplace(entity_handle);

            // Store EntityData in quick-access containers.
            EntityDataComponent const& data_component = _entt_registry.get<EntityDataComponent>(entity_handle);
            _entity_duid_register.emplace(data_component.id, entity_handle);
            _entity_name_register.emplace(data_component.name, entity_handle);
        }

        void EntityRegistry::UnregisterEntity(InternalEntity const& a_entity)
        {
            DFW_ASSERT(a_entity.IsEntityValid());

            // Remove Entity Handle.
            EntityHandle const entity_handle = a_entity.GetHandle();
            _registered_entities.erase(entity_handle);

            // Remove EntityData in quick-access containers.
            EntityDataComponent const& data_component = _entt_registry.get<EntityDataComponent>(entity_handle);
            _entity_duid_register.erase(data_component.id);
            _entity_name_register.erase(data_component.name);
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
