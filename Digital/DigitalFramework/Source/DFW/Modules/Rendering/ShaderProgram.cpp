#include <Modules/Rendering/ShaderProgram.h>

namespace DFW
{
    namespace DRender
    {
       ShaderProgram::ShaderProgram(bgfx::ProgramHandle const a_shader_program_handle, std::vector<SharedPtr<ShaderObject>> const& a_shaders)
           : shader_program_handle(a_shader_program_handle)
           , shaders(a_shaders)
       {
       }

       ShaderProgram::operator bgfx::ProgramHandle()
       {
           return shader_program_handle;
       }

       ShaderProgram::operator bgfx::ProgramHandle() const
       {
           return shader_program_handle;
       }

       bool ShaderProgram::operator==(ShaderProgram const& a_other)
       {
           return this->shader_program_handle.idx == a_other.shader_program_handle.idx;
       }

       bool ShaderProgram::operator!=(ShaderProgram const& a_other)
       {
           return !(*this == a_other);
       }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
