#pragma once

#include <DFW/Modules/Rendering/ShaderObject.h>

#include <bgfx/bgfx.h>

#include <vector>

namespace DFW
{
    namespace DRender
    {
        struct ShaderProgram
        {
            ShaderProgram() = default;
            ShaderProgram(bgfx::ProgramHandle const a_shader_program_handle, std::vector<SharedPtr<ShaderObject>> const& a_shaders);

            operator bgfx::ProgramHandle();
            operator bgfx::ProgramHandle() const;

            bool operator==(ShaderProgram const& a_other);
            bool operator!=(ShaderProgram const& a_other);

            bgfx::ProgramHandle shader_program_handle;
            std::vector<SharedPtr<ShaderObject>> shaders;

        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
