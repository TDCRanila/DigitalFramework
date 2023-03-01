#include <Modules/ECS/Objects/InternalEntity.h>

#include <Modules/ECS/Objects/ECSEntityRegistry.h>

namespace DFW
{
    namespace DECS
    {
        InternalEntity::InternalEntity()
            : _handle(DFW_NULL_ENTITY_HANDLE)
            , _registry(nullptr)
        {
        }

        InternalEntity::InternalEntity(EntityHandle const a_handle, EntityRegistry* a_registry)
            : _handle(a_handle)
            , _registry(a_registry)
        {
            DFW_ASSERT(IsValidEntityHandle(_handle));
            DFW_ASSERT(_registry && _registry->IsValid());
        }

        bool InternalEntity::IsEntityValid() const
        {
            if (!IsValidEntityHandle(_handle))
                return false;

            if (!_registry)
                return false;

            if (!_registry->registry.valid(_handle))
                return false;

            return true;
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
