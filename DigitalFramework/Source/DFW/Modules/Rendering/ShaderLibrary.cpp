#include <DFW/Modules/Rendering/ShaderLibrary.h>

#include <DFW/CoreSystems/Logging/Logger.h>
#include <DFW/Utility/FileSystemUtility.h>

#include <DFW/Modules/Rendering/ShaderProgram.h>
#include <DFW/Modules/Rendering/ShaderObject.h>

#include <bgfx/bgfx.h>

#include <algorithm>

namespace DFW
{
    namespace DRender
    {
		namespace Detail
		{
			std::string const GetShaderType()
			{
				std::string shader_type;
				bgfx::RendererType::Enum const render_type = bgfx::getRendererType();
				switch (render_type)
				{
					case bgfx::RendererType::Noop:
					case bgfx::RendererType::Direct3D11:
					case bgfx::RendererType::Direct3D12: shader_type = "dx11";  break;
					case bgfx::RendererType::Agc:
					case bgfx::RendererType::Gnm:        shader_type = "pssl";  break;
					case bgfx::RendererType::Metal:      shader_type = "metal"; break;
					case bgfx::RendererType::Nvn:        shader_type = "nvn";   break;
					case bgfx::RendererType::OpenGL:     shader_type = "glsl";  break;
					case bgfx::RendererType::OpenGLES:   shader_type = "essl";  break;
					case bgfx::RendererType::Vulkan:     shader_type = "spirv"; break;

					case bgfx::RendererType::Count:						
						DFW_ERRORLOG("Failed to load shader, as the requested type is not supported." /* RenderAPI:" // TODO: STRINGIFY ENUM; render_type*/);
						DFW_ASSERT(false);
						break;
				}

				return shader_type;
			}

			bgfx::ShaderHandle LoadAndCreateShader(std::string const& a_shader_filepath)
			{
				// Read shader file data.
				size_t buffer_size;
				uint8* const loaded_shader_data = DUtility::ReadBinaryFileIntoBuffer(buffer_size, a_shader_filepath);
				if (!loaded_shader_data)
				{
					DFW_ERRORLOG("Attempted to read shader file data into buffer, but failed. File; {}", a_shader_filepath);
					return bgfx::ShaderHandle();
				}

				// "Convert" shader buffer into bgfx buffer.
				bgfx::Memory const* shader_data = bgfx::copy(loaded_shader_data, static_cast<uint32>(buffer_size));
				delete loaded_shader_data;

				bgfx::ShaderHandle const shader_handle = bgfx::createShader(shader_data);
				if (!bgfx::isValid(shader_handle))
				{
					DFW_ERRORLOG("Attempted to create a shader, but failed. File; {}", a_shader_filepath);
					return bgfx::ShaderHandle();
				}
				
				return shader_handle;
			}
		} // End of namespace ~ Detail.

		void ShaderLibrary::FreeLibraryResources()
		{
			for (auto const& shader_program_ptr : _shader_programs)
			{
				bgfx::destroy(shader_program_ptr->shader_program_handle);
				shader_program_ptr->shader_program_handle = BGFX_INVALID_HANDLE;
			}

			for (auto const& [shader_name, shader_ptr]: _shaders)
			{
				bgfx::destroy(shader_ptr->handle);
				shader_ptr->handle = BGFX_INVALID_HANDLE;
			}

			_shader_programs.clear();
			_shaders.clear();
		}

		SharedPtr<ShaderProgram> ShaderLibrary::ConstructProgram(std::string const& a_vs_file_name)
		{
			return ConstructProgram(a_vs_file_name, "");
		}

        SharedPtr<ShaderProgram> ShaderLibrary::ConstructProgram(std::string const& a_vs_file_name, std::string const& a_fs_file_name)
        {
			DFW_LOG("Creating new shader program with - VS; {} and FS; {}", a_vs_file_name, a_fs_file_name);

			// Check if program has already been created.
			if (SharedPtr<ShaderProgram> const& program = GetProgramIfStored(a_vs_file_name, a_fs_file_name))
			{
				DFW_LOG("Shader program shader(ID:{}) with specified shader file names was found. ", program->shader_program_handle.idx);
				return program;
			}

			// Validate provided names.
			if (a_vs_file_name.empty())
			{
				DFW_ERRORLOG("Cannot create a new shader program without a vertex shader.");
				return nullptr;
			}

			bool load_fragment_shader(true);
			if (a_fs_file_name.empty())
			{
				load_fragment_shader = false;
				DFW_LOG("Creating a new shader program without a fragment shader.");
			}

			// Gather file strings
			std::string resource_directory;
			DUtility::GetWorkDirectory(resource_directory);
			std::string const& shader_type = Detail::GetShaderType();

			std::string const shader_folder_path(resource_directory + DIR_SLASH_CHAR + DEFAULT_SHADER_FOLDER_NAME + DIR_SLASH_CHAR + shader_type);
			std::string const vs_filepath(shader_folder_path + DIR_SLASH_CHAR + a_vs_file_name + GENERATED_SHADER_FILE_EXTENSION);
			std::string const fs_filepath(shader_folder_path + DIR_SLASH_CHAR + a_fs_file_name + GENERATED_SHADER_FILE_EXTENSION);

			// Load, Create, Store Shaders ~ Vertex.
			SharedPtr<ShaderObject> vs_shader;
			if (IsShaderStored(a_vs_file_name))
			{
				vs_shader = _shaders.at(a_vs_file_name);
			}
			else
			{
				vs_shader = MakeShared<ShaderObject>();
				if (DUtility::DoesFileExist(vs_filepath))
					vs_shader->handle = Detail::LoadAndCreateShader(vs_filepath);

				if (!bgfx::isValid(vs_shader->handle))
				{
					DFW_ERRORLOG("Failed to create a new vertex shader object with; {}", vs_filepath);
					return nullptr;
				}

				vs_shader->name				= a_vs_file_name;
				vs_shader->shader_filepath	= vs_filepath;
				vs_shader->shader_type		= ShaderType::VERTEX;

				_shaders.emplace(a_vs_file_name, vs_shader);
			}
			
			// Load, Create, Store Shaders ~ Fragment.
			SharedPtr<ShaderObject> fs_shader;
			if (load_fragment_shader)
			{
				if (IsShaderStored(a_fs_file_name))
				{
					fs_shader = _shaders.at(a_fs_file_name);
				}
				else
				{
					fs_shader = MakeShared<ShaderObject>();
					if (DUtility::DoesFileExist(fs_filepath))
						fs_shader->handle = Detail::LoadAndCreateShader(fs_filepath);

					if (!bgfx::isValid(fs_shader->handle))
					{
						DFW_ERRORLOG("Failed to create a new fragment shader object with; {}", fs_filepath);
						return nullptr;
					}

					fs_shader->name				= a_fs_file_name;
					fs_shader->shader_filepath	= fs_filepath;
					fs_shader->shader_type		= ShaderType::FRAGMENT;

					_shaders.emplace(a_fs_file_name, fs_shader);
				}
			}

			// Construct Shader Program.
			bgfx::ProgramHandle const shader_program = bgfx::createProgram(vs_shader->handle
				, (load_fragment_shader ? fs_shader->handle : bgfx::ShaderHandle())
				, false
			);
			
			if (!bgfx::isValid(shader_program))
			{
				DFW_ERRORLOG("Failed to create shader program with - VS; {} and FS; {}", vs_shader->handle.idx, fs_shader->handle.idx);
				return nullptr;
			}

			// Store Shader Program.
			SharedPtr<ShaderProgram> program = MakeShared<ShaderProgram>();
			program->shader_program_handle = shader_program;
			program->shaders = load_fragment_shader ? std::vector<SharedPtr<ShaderObject>>{ vs_shader, fs_shader} : std::vector<SharedPtr<ShaderObject>>{ vs_shader };
			
			_shader_programs.emplace_back(program);
			
			DFW_INFOLOG("Successfully created new shader program(ID: {})", program->shader_program_handle.idx);

			return program;
        }

        void ShaderLibrary::DestroyProgram(SharedPtr<ShaderProgram> const& a_shader_program)
        {
			if (!bgfx::isValid(a_shader_program->shader_program_handle))
				return;

			if (auto const& it = std::find(_shader_programs.begin(), _shader_programs.end(), a_shader_program); it != _shader_programs.end())
			{
				bgfx::destroy(a_shader_program->shader_program_handle);
				a_shader_program->shader_program_handle = BGFX_INVALID_HANDLE;
				_shader_programs.erase(it);
			}
        }

		bool ShaderLibrary::IsShaderStored(std::string const& a_shader_name) const
		{
			if (auto const& it = _shaders.find(a_shader_name); it != _shaders.end())
			{
				SharedPtr<ShaderObject> const& shader = it->second;
				if (shader && bgfx::isValid(shader->handle))
					return true;
			}

			return false;
		}

		SharedPtr<ShaderProgram> ShaderLibrary::GetProgramIfStored(std::string const& a_vs_file_name) const
		{
			return GetProgramIfStored(a_vs_file_name, "");
		}

		SharedPtr<ShaderProgram> ShaderLibrary::GetProgramIfStored(std::string const& a_vs_file_name, std::string const& a_fs_file_name) const
		{
			auto const compare = [&a_vs_file_name, &a_fs_file_name](SharedPtr<ShaderProgram> const& a_shader_program) -> bool
			{
				bool is_same(true);
				for (auto const& shader : a_shader_program->shaders)
				{
					if (shader->shader_type == ShaderType::VERTEX)		{ is_same = (shader->name != a_vs_file_name) ? false : is_same; continue; }
					if (shader->shader_type == ShaderType::FRAGMENT)	{ is_same = (shader->name != a_fs_file_name) ? false : is_same; continue; }
				}

				return is_same;
			};

			if (auto const& it = std::find_if(_shader_programs.begin(), _shader_programs.end(), compare); it != _shader_programs.end())
				return *it;
			else
				return nullptr;
		}

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
