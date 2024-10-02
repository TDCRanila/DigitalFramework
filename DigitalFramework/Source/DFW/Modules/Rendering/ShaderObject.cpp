#include <DFW/Modules/Rendering/ShaderObject.h>

namespace DFW
{
    namespace DRender
    {
        ShaderObject::ShaderObject(std::string const& a_shader_name, std::string const& a_shader_filepath, bgfx::ShaderHandle const a_shader_handle, ShaderType const a_shader_type)
            : name(a_shader_name)
            , shader_filepath(a_shader_filepath)
            , handle(a_shader_handle)
            , shader_type(a_shader_type)
        {
        }

        ShaderObject::operator bgfx::ShaderHandle()
        {
            return handle;
        }

        ShaderObject::operator bgfx::ShaderHandle() const
        {
            return handle;
        }


    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
