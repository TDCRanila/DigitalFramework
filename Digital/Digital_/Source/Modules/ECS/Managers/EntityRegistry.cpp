#include <Modules/ECS/Managers/EntityRegistry.h>

#include <Modules/ECS/Internal/EntityDataComponent.h>
#include <Modules/ECS/Entity.h>

namespace DFW
{
    namespace DECS
    {
        EntityRegistry::EntityRegistry(std::string const& a_registry_name)
            : id(DFW::GenerateDUID())
            , name(a_registry_name)
        {
            registry.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);

            _entities.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
            _pending_deletion_entities.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
            
            _entity_handle_registration.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
            _entity_duid_registration.reserve(DFW_REGISTRY_ENTITY_RESERVATION_SIZE);
        }

        EntityRegistry::~EntityRegistry()
        {
            registry.clear();
            
            _entities.clear();
            _pending_deletion_entities.clear();

            _entity_handle_registration.clear();
            _entity_duid_registration.clear();
        }

        std::vector<Entity> EntityRegistry::GetEntities()
        {
            //// TODO Not using ranges/view as of right now due to C20.
            //auto range = _entities | std::ranges::views::transform([this](EntityHandle const& a_handle) { return Entity(a_handle, this); });
            //// TODO: std::ranges::to_vector is not part of C20, so using a custom implementation instead.
            //std::vector<std::ranges::range_value_t<decltype(range)>> v;
            //v.reserve(_entities.size());
            //std::ranges::copy(range, std::back_inserter(v));
            // return v;
            
            std::vector<Entity> entities;
            for (auto const& entity_handle : _entities)
            {
                entities.emplace_back(entity_handle, *this);
            }
            return entities;
        }

        bool EntityRegistry::IsValid() const
        {
            return id != DFW_INVALID_DUID;
        }

        void EntityRegistry::RegisterEntity(InternalEntity const& a_entity, DFW::RefWrap<EntityDataComponent> a_registration_comp)
        {
            /*DFW_ASSERT(a_entity.IsEntityValid());*/
            EntityHandle const entity_handle = a_entity.GetHandle();
            _entities.emplace(entity_handle);
            _entity_handle_registration.emplace(entity_handle, a_registration_comp);
            _entity_duid_registration.emplace(registry.get<EntityDataComponent>(entity_handle).id, entity_handle);
        }

        void EntityRegistry::UnregisterEntity(InternalEntity const& a_entity)
        {
            /*DFW_ASSERT(a_entity.IsEntityValid());*/
            EntityHandle const entity_handle = a_entity.GetHandle();
            _entities.erase(entity_handle);
            _entity_handle_registration.erase(entity_handle);
            _entity_duid_registration.erase(registry.get<EntityDataComponent>(entity_handle).id);
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
