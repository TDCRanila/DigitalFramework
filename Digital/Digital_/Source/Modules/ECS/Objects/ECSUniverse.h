#pragma once

#include <CoreSystems/DUID.h>
#include <CoreSystems/Memory.h>

#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>

#include <entt/entity/registry.hpp>

#include <unordered_set>
#include <unordered_map>
#include <functional>

namespace DFW
{
    namespace DECS
    {
        // FW Declare
        class EntityManager;
        class ComponentManager;
        class Entity;

        const int64 DFW_UNIVERSE_ENTITY_RESERVATION_SIZE = 256;

        using EntityRegistrationMap = std::unordered_map<EntityHandle, DFW::RefWrap<EntityRegistrationComponent>>;

        class Universe final
        {
        private:
            friend EntityManager;
            friend ComponentManager;
            friend Entity;

        public:
            Universe();
            ~Universe();

            bool IsValid() const;

            std::strong_ordering operator<=>(Universe const& a_other) const = default;

            std::vector<Entity> GetEntities();

            entt::registry  registry;
            DFW::DUID const id;

        private:
            EntityRegistrationMap               _entity_data_registration;
            std::unordered_set<EntityHandle>    _entities;
            std::unordered_set<EntityHandle>    _pending_deletion_entities;

        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
