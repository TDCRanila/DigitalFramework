#pragma once

namespace DFW
{
    struct ClipSpace
    {
        ClipSpace();
        ClipSpace(float32 a_clip_near, float32 a_clip_far);

        float32 z_near;
        float32 z_far;
    };

} // End of namespace ~ DFW.
