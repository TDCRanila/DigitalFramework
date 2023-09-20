#include <GameWorld/Camera/CameraComponent.h>

namespace DFW
{
    CameraComponent::CameraComponent()
        : projection(1.0f)
        , view(1.0f)

        , front(0.0f, 0.0f, 1.0f)
        , up(0.0f, 1.0f, 0.0)
        , right(1.0f, 0.0f, 0.0f)

        , world_front(0.0f, 0.0f, 1.0f)
        , world_up(0.0f, 1.0f, 0.0)
        , world_right(1.0f, 0.0f, 0.0f)

        , angles(0.0f)

        , clip(0.1f, 1000.f)
        , fov(DFW_DEFAULT_CAMERA_FOV)
        , fly_speed(DFW_DEFAULT_CAMERA_SPEED)

        , has_enabled_controls(false)
        , has_enabled_six_degrees_rotation(false)

        , name("")
    {
    }

} // End of namespace ~ DFW.
