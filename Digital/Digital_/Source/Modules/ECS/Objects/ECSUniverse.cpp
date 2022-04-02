#include <Modules/ECS/Objects/ECSUniverse.h>

#include <ranges>

namespace DFW
{
    namespace DECS
    {
        Universe::Universe(DFW::DUID a_universe_id, std::string const& a_universe_name)
            : id(a_universe_id)
            , name(a_universe_name)
        {
            registry.reserve(DFW_UNIVERSE_ENTITY_RESERVATION_SIZE);
            _entity_data_registration.reserve(DFW_UNIVERSE_ENTITY_RESERVATION_SIZE);
            _entities.reserve(DFW_UNIVERSE_ENTITY_RESERVATION_SIZE);
            _pending_deletion_entities.reserve(DFW_UNIVERSE_ENTITY_RESERVATION_SIZE);
        }

        Universe::~Universe()
        {
            registry.clear();
            _entity_data_registration.clear();
            _entities.clear();
            _pending_deletion_entities.clear();
        }

        bool Universe::IsValid() const
        {
            return id != DFW_INVALID_DUID;
        }

        std::vector<Entity>  Universe::GetEntities()
        {
            //// TODO Not using ranges/view as of right now due to C20.
            //auto range = _entities | std::ranges::views::transform([this](EntityHandle const& a_handle) { return Entity(a_handle, this); });
            //// TODO: std::ranges::to_vector is not part of C20, so using a custom implementation instead.
            //std::vector<std::ranges::range_value_t<decltype(range)>> v;
            //v.reserve(_entities.size());
            //std::ranges::copy(range, std::back_inserter(v));
            // return v;
            
            std::vector<Entity> entities;
            for (auto const& entity_handle : entities)
            {
                entities.emplace_back(entity_handle);
            }
            return entities;
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
