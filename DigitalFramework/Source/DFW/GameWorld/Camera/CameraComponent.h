#pragma once

#include <DFW/GameWorld/Camera/ClipSpace.h>

#include <DFW/Modules/ECS/Component.h>
#include <DFW/Modules/Math/EulerAngles.h>

#include <DFW/CoreSystems/DUID.h>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>

namespace DFW
{
    constexpr float32 DFW_DEFAULT_CAMERA_FOV(90.f);
    constexpr float32 DFW_DEFAULT_CAMERA_SPEED(50.f);

    struct CameraComponent : public DECS::Component::StrictRegistrar<CameraComponent>
    {
        CameraComponent();

        glm::mat4   projection;
        glm::mat4   view;

        glm::vec3   front;
        glm::vec3   up;
        glm::vec3   right;

        glm::vec3   world_front;
        glm::vec3   world_up;
        glm::vec3   world_right;

        DMath::EulerAngles angles; // Pitch(x), Yaw(y), Roll(z); in degrees.

        ClipSpace   clip;
        float32     fov;
        float32     fly_speed;

        bool has_enabled_controls;
        bool has_enabled_six_degrees_rotation;
        
        std::string name;

    };


} // End of namespace ~ DFW.
