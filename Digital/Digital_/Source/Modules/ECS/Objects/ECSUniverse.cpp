#include <Modules/ECS/Objects/ECSUniverse.h>

#include <ranges>

namespace DECS
{
    Universe::Universe()
        : _id(DCore::GenerateDUID())
    {
        _registry.reserve(DFW_UNIVERSE_ENTITY_RESERVATION_SIZE);
        _entity_data_registration.reserve(DFW_UNIVERSE_ENTITY_RESERVATION_SIZE);
        _entities.reserve(DFW_UNIVERSE_ENTITY_RESERVATION_SIZE);
        _deleted_entities.reserve(DFW_UNIVERSE_ENTITY_RESERVATION_SIZE);
    }

    Universe::~Universe()
    {
        _registry.clear();
        _entity_data_registration.clear();
        _entities.clear();
        _deleted_entities.clear();
    }

    std::vector<Entity>  Universe::GetEntities()
    {
        auto range = _entities | std::ranges::views::transform([this](EntityHandle const& a_handle) { return Entity(a_handle, this); } );
        
        // TODO: std::ranges::to_vector is not part of C20, so using a custom implementation instead.
        std::vector<std::ranges::range_value_t<decltype(range)>> v;
        v.reserve(_entities.size());
        std::ranges::copy(range, std::back_inserter(v));
        
        return v;
    }

    std::vector<EntityHandle> const& Universe::GetEntityHandles() const
    {
        return _entities;
    }

    EntityRegistrationMap const& Universe::GetEntityRegistrationMap() const
    {
        return _entity_data_registration;
    }

} // End of namespace ~ DECS
