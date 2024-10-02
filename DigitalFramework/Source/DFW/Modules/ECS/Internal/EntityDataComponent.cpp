#include <DFW/Modules/ECS/Internal/EntityDataComponent.h>

namespace DFW
{
    namespace DECS
    {
        EntityDataComponent::EntityDataComponent()
            : id(DFW_INVALID_DUID)
            , type(DFW_INVALID_ENTITY_TYPE_ID)
            , name(DFW_DEFAULT_ENTITY_NAME)
        {
        }

        EntityDataComponent::EntityDataComponent(DFW::DUID const a_id, EntityTypeID const a_type, std::string const& a_name)
            : id(a_id)
            , type(a_type)
            , name(a_name)
        {
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
