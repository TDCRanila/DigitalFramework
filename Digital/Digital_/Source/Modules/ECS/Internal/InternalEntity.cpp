#include <Modules/ECS/Internal/InternalEntity.h>

#include <Modules/ECS/Managers/EntityRegistry.h>

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

            if (!_registry->ENTT().valid(_handle))
                return false;

            return true;
        }

        bool InternalEntity::IsMarkedForDestruction() const
        {
            std::unordered_set<EntityHandle> const& marked_entities = _registry->_marked_entities_for_destruction;
            if (auto const& it = marked_entities.find(_handle); it != marked_entities.end())
                return true;
            else
                return false;
        }

    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.
