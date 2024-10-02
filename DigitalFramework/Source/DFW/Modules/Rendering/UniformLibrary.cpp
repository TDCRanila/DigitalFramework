#include <Modules/Rendering/UniformLibrary.h>

namespace DFW
{
    namespace DRender
    {
        namespace Detail
        {
            bgfx::UniformType::Enum ConvertToBGFXUniformType(UniformTypes const a_uniform_type)
            {
                switch (a_uniform_type)
                {
                    case(UniformTypes::Sampler): return bgfx::UniformType::Sampler; break;
                    case(UniformTypes::Mat3): return bgfx::UniformType::Mat3; break;
                    case(UniformTypes::Mat4): return bgfx::UniformType::Mat4; break;
                    case(UniformTypes::Vec4): return bgfx::UniformType::Vec4; break;
                    default: DFW_ASSERT(false && "Unrecognised uniform detected."); return bgfx::UniformType::End;  break;
                }
            }
        } // End of namespace ~ Detail.

        SharedPtr<Uniform> UniformLibrary::CreateUniform(std::string const& a_uniform_name, UniformTypes const a_uniform_type)
        {
            if (auto const& it = _uniform_map.find(a_uniform_name); it != _uniform_map.end())
            {
                return it->second;
            }
            else
            {
                auto const& [constructed, result] = _uniform_map.emplace(a_uniform_name, MakeShared<Uniform>());
                SharedPtr<Uniform>& uniform(constructed->second);
                uniform->handle = bgfx::createUniform(a_uniform_name.c_str(), Detail::ConvertToBGFXUniformType(a_uniform_type));
                DFW_ASSERT(bgfx::isValid(uniform->handle) && "Something went wrong when trying create a uniform.");
                uniform->type = a_uniform_type;
                bgfx::getUniformInfo(uniform->handle, uniform->info);

                DFW_INFOLOG("Creating Shader Uniform with name: {}", a_uniform_name);
                return uniform;
            }
        }
        
        void UniformLibrary::DestroyUniform(Uniform const& a_uniform)
        {
            if (auto const& it = _uniform_map.find(a_uniform.info.name); it != _uniform_map.end())
            {
                DFW_INFOLOG("Destroying Shader Uniform with name: {}", a_uniform.info.name);
                bgfx::destroy(a_uniform.handle);
                _uniform_map.erase(it);
            }
        }

        bool UniformLibrary::IsUniformPresent(Uniform const& a_uniform) const
        {
            return _uniform_map.find(a_uniform.info.name) != _uniform_map.end() ? true : false;
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
