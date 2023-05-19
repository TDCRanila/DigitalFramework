#pragma once

#include <CoreSystems/Memory.h>
#include <CoreSystems/DUID.h>

#include <unordered_map>
#include <vector>

namespace DFW
{
    namespace DRender
    {
        // FW Declare.
        struct ShaderObject;
        struct ShaderProgram;

        using ShaderProgramID = DFW::DUID;

        class ShaderLibrary
        {
        public:
            static constexpr char const* DEFAULT_SHADER_FOLDER_NAME         = "shaders";
            static constexpr char const* GENERATED_SHADER_FILE_EXTENSION    = ".bin";

            ShaderLibrary() = default;
            ~ShaderLibrary() = default;

            void FreeLibraryResources();
            
            // TODO Should use the FilePath class instead of strings.
            // Would need to change shader folder structure/usage.
            // Construct ShaderProgram.
            SharedPtr<ShaderProgram> ConstructProgram(std::string const& a_vs_file_name);
            SharedPtr<ShaderProgram> ConstructProgram(std::string const& a_vs_file_name, std::string const& a_fs_file_name);
            SharedPtr<ShaderProgram> ConstructComputeProgram(std::string const& a_cs_file_name);

            void DestroyProgram(SharedPtr<ShaderProgram> const& a_shader_program);

            // Hotreload shader file - run shaderc on sc file to create bin file and create new shader with file if shared and recreate programme.
            bool ReloadProgram(SharedPtr<ShaderProgram> const& a_shader_program);
                
        private:
            bool IsShaderStored(std::string const& a_shader_name) const;

            SharedPtr<ShaderProgram> GetProgramIfStored(std::string const& a_vs_file_name) const;
            SharedPtr<ShaderProgram> GetProgramIfStored(std::string const& a_vs_file_name, std::string const& a_fs_file_name) const;
            SharedPtr<ShaderProgram> GetComputeProgramIfStored(std::string const& a_vs_file_name) const;

        private:
            std::vector<SharedPtr<ShaderProgram>> _shader_programs;
            std::unordered_map<std::string, SharedPtr<ShaderObject>> _shaders;
        };        

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
