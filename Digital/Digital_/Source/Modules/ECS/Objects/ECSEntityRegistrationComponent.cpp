#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>

namespace DFW
{
    namespace DECS
    {
        EntityDataComponent::EntityDataComponent()
            : id(DFW_INVALID_DUID)
            , comp_list(0)
            , type(DFW_INVALID_ENTITY_TYPE_ID)
            , name(DFW_DEFAULT_ENTITY_NAME)
        {
        }

        EntityDataComponent::EntityDataComponent(DFW::DUID const a_id, EntityTypeID const a_type, std::string const& a_name)
            : id(a_id)
            , comp_list(0)
            , type(a_type)
            , name(a_name)
        {
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
