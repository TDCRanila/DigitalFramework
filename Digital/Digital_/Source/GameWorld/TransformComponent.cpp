#include <GameWorld/TransformComponent.h>

namespace DFW
{
    TransformComponent::TransformComponent(Transform const& a_transform)
    {
        translation = a_transform.translation;
        rotation    = a_transform.rotation;
        scale       = a_transform.scale;
    }

} // End of namespace ~ DFW.
