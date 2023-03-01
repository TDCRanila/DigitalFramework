#pragma once

#include <Modules/ECS/Objects/ECSEntityHandle.h>
#include <Modules/ECS/Objects/InternalEntity.h>

#include <CoreSystems/DUID.h>

#include <entt/entity/registry.hpp>

#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace DFW
{
    namespace DECS
    {
        // FW Declare
        class EntityManager;
        class Entity;
        struct EntityDataComponent;

        const int64 DFW_REGISTRY_ENTITY_RESERVATION_SIZE = 256;

        using EntityHandleRegistrationMap   = std::unordered_map<EntityHandle, DFW::RefWrap<EntityDataComponent>>;
        using EntityDUIDRegistrationMap     = std::unordered_map<DFW::DUID, EntityHandle>;

        class EntityRegistry final
        {
        private:
            friend EntityManager;
            friend Entity;

        public:
            EntityRegistry(std::string const& a_registry_name);
            ~EntityRegistry();

            std::strong_ordering operator<=>(EntityRegistry const& a_other) const = default;
            
            std::vector<Entity> GetEntities();

            bool IsValid() const;

            entt::registry  registry;
            DFW::DUID const id;
            std::string const name;

        private:
            void RegisterEntity(InternalEntity const& a_entity, DFW::RefWrap<EntityDataComponent> a_registration_comp);
            void UnregisterEntity(InternalEntity const& a_entity);

            EntityHandleRegistrationMap         _entity_handle_registration;
            EntityDUIDRegistrationMap           _entity_duid_registration;
            std::unordered_set<EntityHandle>    _entities;
            std::unordered_set<EntityHandle>    _pending_deletion_entities;

        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
