#include <Modules/ECS/Objects/ECSEntityRegistrationComponent.h>

namespace DFW
{
    namespace DECS
    {
        EntityRegistrationComponent::EntityRegistrationComponent()
            : name(DFW_DEFAULT_ENTITY_NAME)
            , id(DFW_INVALID_DUID)
            , comp_list(0)
        {
        }

        EntityRegistrationComponent::EntityRegistrationComponent(DFW::DUID const a_id, std::string const& a_name)
            : name(a_name)
            , id(a_id)
            , comp_list(0)
        {
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
