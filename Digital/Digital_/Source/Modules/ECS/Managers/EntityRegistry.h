#pragma once

#include <Modules/ECS/EntityHandle.h>

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
        class ECSModule;
        class EntityManager;
        class Entity;

        constexpr int64 DFW_REGISTRY_ENTITY_RESERVATION_SIZE = 256;

        using EntityDUIDRegisterMap = std::unordered_map<DFW::DUID, EntityHandle>;
        using EntityNameRegisterMap = std::unordered_map<std::string, EntityHandle>;

        class EntityRegistry final
        {
        private:
            friend ECSModule;
            friend EntityManager;
            friend Entity;

        public:
            EntityRegistry();
            ~EntityRegistry();
            
            DFW::DUID GetID() const { return _id; }
            entt::registry& ENTT() { return _entt_registry; }

        public:
            Entity GetEntity(DFW::DUID const a_entity_id);
            Entity GetEntity(std::string const& a_entity_name);
            std::vector<Entity> GetEntities();

            bool IsEntityMarkedForDestruction(EntityHandle const a_entity_handle) const;

        private:
            void CleanDestructionMarkedEntities();
            void RegisterEntity(EntityHandle const a_entity_handle);
            void UnregisterEntity(EntityHandle const a_entity_handle);

        private:
            entt::registry _entt_registry;
            DFW::DUID _id;

            EntityDUIDRegisterMap _entity_duid_register;
            EntityNameRegisterMap _entity_name_register;
            std::unordered_set<EntityHandle> _marked_entities_for_destruction;

        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
