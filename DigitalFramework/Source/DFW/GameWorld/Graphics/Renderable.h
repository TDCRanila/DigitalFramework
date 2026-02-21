#pragma once

#include <DFW/Utility/ColourUtility.h>

namespace DFW
{
    struct Renderable
    {
        Renderable();

        bool is_visible;
        ColourRGBA colour;
    };

} // End of namespace ~ DFW.
