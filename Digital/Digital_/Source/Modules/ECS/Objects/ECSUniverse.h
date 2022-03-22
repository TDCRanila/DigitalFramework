#pragma once

#include <CoreSystems/DUID.h>
#include <CoreSystems/Memory.h>

#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>

#include <entt/entity/registry.hpp>

#include <vector>
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

        public:
            Universe();
            ~Universe();

            std::strong_ordering operator<=>(Universe const& a_other) const = default;

            std::vector<Entity> GetEntities();
            std::vector<EntityHandle> const& GetEntityHandles() const;
            EntityRegistrationMap const& GetEntityRegistrationMap() const;

            entt::registry      _registry;
            DFW::DUID const   _id;

        protected:
            friend EntityManager;
            friend ComponentManager;
            friend Entity;

            // TODO Could use a pool alocator here.
            // TODO Can remove _entities container and replace GetEntityHandles with a range.
            EntityRegistrationMap       _entity_data_registration;
            std::vector<EntityHandle>   _entities;
            std::vector<EntityHandle>   _deleted_entities;

        };

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
