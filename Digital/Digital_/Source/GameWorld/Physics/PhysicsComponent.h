#pragma once

#include <Modules/ECS/Component.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

namespace DFW
{
    struct RigidBodyComponent : Component::StrictRegistrar<RigidBodyComponent>
    {
        RigidBodyComponent()
            : motion_type(JPH::EMotionType::Dynamic) 
        {}

        virtual ~RigidBodyComponent() = default;

        JPH::BodyID body_id;
        JPH::EMotionType motion_type;

    };

} // End of namespace ~ DFW.
