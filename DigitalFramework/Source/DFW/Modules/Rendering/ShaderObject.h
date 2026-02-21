#pragma once

#include <bgfx/bgfx.h>

namespace DFW
{
    namespace DRender
    {
        enum class ShaderType
        {
                VERTEX
            ,   FRAGMENT
            ,   COMPUTE
            ,   NONE
        };

        struct ShaderObject
        {
            ShaderObject() = default;
            ShaderObject(std::string const& a_shader_name, std::string const& a_shader_filepath, bgfx::ShaderHandle const a_shader_handle, ShaderType const a_shader_type);

            operator bgfx::ShaderHandle();
            operator bgfx::ShaderHandle() const;

            std::string         name;
            std::string         shader_filepath;
            bgfx::ShaderHandle  handle;
            ShaderType          shader_type;

        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
