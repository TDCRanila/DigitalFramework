#pragma once

#include <bgfx/bgfx.h>

namespace DFW
{
    namespace DRender
    {
        enum class UniformTypes
        {
                Sampler
            ,   Vec4
            ,   Mat3
            ,   Mat4
        };

        struct Uniform
        {
            Uniform() = default;
            ~Uniform() = default;

            bgfx::UniformInfo info;
            bgfx::UniformHandle handle;
            UniformTypes type;
        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
