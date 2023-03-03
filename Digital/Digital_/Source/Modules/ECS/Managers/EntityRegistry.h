#pragma once

#include <Modules/ECS/EntityHandle.h>
#include <Modules/ECS/Internal/InternalEntity.h>

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

        constexpr int64 DFW_REGISTRY_ENTITY_RESERVATION_SIZE = 256;

        using EntityDUIDRegisterMap = std::unordered_map<DFW::DUID, EntityHandle>;
        using EntityNameRegisterMap = std::unordered_map<std::string, EntityHandle>;

        class EntityRegistry final
        {
        private:
            friend EntityManager;
            friend Entity;
            friend InternalEntity;

        public:
            EntityRegistry(std::string const& a_registry_name);
            ~EntityRegistry();

            std::strong_ordering operator<=>(EntityRegistry const& a_other) const = default;
            
        public:
            entt::registry& ENTT() { return _entt_registry; }

            Entity GetEntity(DFW::DUID const a_entity_id);
            Entity GetEntity(std::string const& a_entity_name);
            std::vector<Entity> GetEntities();

            bool IsValid() const;
            DFW::DUID GetID() const { return _id; }
            std::string GetName() const { return _name; };

        private:
            void RegisterEntity(InternalEntity const& a_entity);
            void UnregisterEntity(InternalEntity const& a_entity);

        private:
            entt::registry _entt_registry;
            DFW::DUID _id;
            std::string _name;

            EntityDUIDRegisterMap _entity_duid_register;
            EntityNameRegisterMap _entity_name_register;
            std::unordered_set<EntityHandle> _registered_entities;
            std::unordered_set<EntityHandle> _marked_entities_for_destruction;

        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
