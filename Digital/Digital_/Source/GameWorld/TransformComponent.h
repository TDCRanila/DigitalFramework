#pragma once

#include <GameWorld/Transform.h>

#include <Modules/ECS/Objects/ECSComponent.h>

namespace DFW
{
    struct TransformComponent : public Transform, DECS::Component::StrictRegistrar<TransformComponent>
    {
        TransformComponent() = default;
        TransformComponent(Transform const& a_transform);

    };

} // End of namespace ~ DFW.
