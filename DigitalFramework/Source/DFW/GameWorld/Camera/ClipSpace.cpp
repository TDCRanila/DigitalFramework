#include <DFW/GameWorld/Camera/ClipSpace.h>

namespace DFW
{
    ClipSpace::ClipSpace()
        : z_near(0.0f)
        , z_far(0.0f)
    {
    }

    ClipSpace::ClipSpace(float32 a_clip_near, float32 a_clip_far)
        : z_near(a_clip_near)
        , z_far(a_clip_far)
    {
    }

} // End of namespace ~ DFW.
