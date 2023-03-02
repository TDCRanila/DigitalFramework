#pragma once

#include <GameWorld/Camera/ClipSpace.h>

#include <Modules/ECS/Component.h>
#include <Modules/Math/EulerAngles.h>

#include <CoreSystems/DUID.h>

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

        DMath::EulerAngles  angles;
        glm::quat           orientation;

        ClipSpace   clip;
        float32     fov;
        float32     fly_speed;
        bool        has_enabled_six_degrees_rotation;
        
        std::string name;

    };


} // End of namespace ~ DFW.
