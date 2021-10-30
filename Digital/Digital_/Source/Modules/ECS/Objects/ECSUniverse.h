#pragma once

#include <CoreSystems/DUID.h>

#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>

#include <entt/entity/registry.hpp>

#include <vector>
#include <unordered_map>

namespace DECS
{
    // FW Declare
    class ECSEntityManager;
    class ECSComponentManager;
    class ECSEntity;

    const int64 DFW_UNIVERSE_ENTITY_RESERVATION_SIZE = 256;

    using EntityRegistrationMap = std::unordered_map<EntityHandle, ECSEntityRegistrationComponent*>;

    class ECSUniverse final
    {

    public:
        ECSUniverse();
        ~ECSUniverse();
        
        std::strong_ordering operator<=>(ECSUniverse const& a_other) const = default;

        std::vector<ECSEntity> GetEntities();
        std::vector<EntityHandle> const& GetEntityHandles() const;
        EntityRegistrationMap const& GetEntityRegistrationMap() const;

        entt::registry      _registry;
        DCore::DUID const   _id;
        
    protected:
        friend ECSEntityManager;
        friend ECSComponentManager;
        friend ECSEntity;

        // TODO Could use a pool alocator here.
        // TODO Can remove _entities container and replace GetEntityHandles with a range.
        EntityRegistrationMap       _entity_data_registration;
        std::vector<EntityHandle>   _entities;
        std::vector<EntityHandle>   _deleted_entities;

    };

} // End of namespace ~ DECS.
