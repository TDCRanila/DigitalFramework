#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/Character.h>

#include <Modules/ECS/Component.h>

#include <CoreSystems/Memory.h>

#include <glm/glm.hpp>

namespace DFW
{
    struct CharacterComponent : public DECS::Component::StrictRegistrar<CharacterComponent>
    {
        JPH::Ref<JPH::Character> character;
        
        glm::vec3 input_move_direction = glm::vec3(0.0f);
        glm::vec3 movement_speed = glm::vec3(10.f);
        float32 ground_clearance_tolerance = 0.05f;
        float32 jump_velocity = 10.0f;
        float32 crouch_modifier = 0.5f;
        bool should_jump = false;
        bool is_crouched = false;
        bool can_air_strafe = true;

    };

} // End of namespace ~ DFW.
