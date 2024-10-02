#pragma once

#include <Modules/ECS/Component.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

namespace DFW
{
    struct RigidBodyComponent : Component::StrictRegistrar<RigidBodyComponent>
    {
        RigidBodyComponent() = default;
        RigidBodyComponent(JPH::BodyID const a_body_id) : body_id(a_body_id) {}
        virtual ~RigidBodyComponent() = default;

        JPH::BodyID body_id;

    };

} // End of namespace ~ DFW.
