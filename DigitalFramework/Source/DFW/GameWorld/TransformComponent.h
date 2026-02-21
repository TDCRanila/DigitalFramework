#pragma once

#include <DFW/GameWorld/Transform.h>

#include <DFW/Modules/ECS/Component.h>

namespace DFW
{
    struct TransformComponent : public Transform, DECS::Component::StrictRegistrar<TransformComponent>
    {
        TransformComponent() = default;
        TransformComponent(Transform const& a_transform);

    };

} // End of namespace ~ DFW.
