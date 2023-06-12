#pragma once

#include <Modules/ECS/EntityHandle.h>
#include <Modules/ECS/Utility/EntityTypeID.h>

#include <CoreSystems/DUID.h>

#include <Utility/TemplateUtility.h>

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
        class Entity;

        constexpr int64 DFW_REGISTRY_ENTITY_RESERVATION_SIZE = 256;

        using EntityDUIDRegisterMap = std::unordered_map<DFW::DUID, EntityHandle>;
        using EntityNameRegisterMap = std::unordered_map<std::string, EntityHandle>;

        constexpr StringLiteral DFW_BASE_ENTITY_FAMILY_TYPE_NAME = "Entity";

        class EntityRegistry final
        {
        private:
            friend ECSModule;
            friend Entity;

        public:
            EntityRegistry();
            ~EntityRegistry();
            
            DFW::DUID GetID() const { return _id; }
            entt::registry& ENTT() { return _entt_registry; }

        public:
            Entity CreateEntity();
            void DestroyEntity(Entity const& a_entity);

            Entity GetEntity(DFW::DUID const a_entity_id);
            Entity GetEntity(std::string const& a_entity_name);
            std::vector<Entity> GetEntities();

            template <StringLiteral entity_type_name>
            EntityTypeID GetEntityTypeID() const;

            bool IsEntityMarkedForDestruction(EntityHandle const a_entity_handle) const;

        private:
            void DestroyEntityAndChilderen(Entity const& a_current_entity);
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

#pragma region Template Function Implementation

        template <StringLiteral entity_type_name>
        EntityTypeID EntityRegistry::GetEntityTypeID() const
        {
            return DUtility::FamilyNameType<DFW_BASE_ENTITY_FAMILY_TYPE_NAME>::GetTypeID<entity_type_name>();
        }

#pragma endregion

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
