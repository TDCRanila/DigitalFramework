#include <CoreSystems/Systems/RenderSystem.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Logging/Logger.h>
#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/Rendering/ViewTargetDirector.h>
#include <Modules/Rendering/RenderModuleInterface.h>

#include <Utility/FileSystemUtility.h>

#include <bgfx/bgfx.h>
#include <bx/math.h>

struct PosColorVertex
{
	float32 m_x;
	float32 m_y;
	float32 m_z;
	uint32 m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16 s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;
bgfx::ProgramHandle m_program;

namespace DFW
{
    void RenderSystem::Init()
    {
		// View Target
		_view_target = CoreService::GetRenderModule()->view_director.GetMainViewTarget();

		// Uniform Layouts
		PosColorVertex::init();

		// Buffers
		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
			, PosColorVertex::ms_layout
		);

		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
		);

		// Shaders		
		std::string resource_directory;
		DUtility::GetWorkDirectory(resource_directory);
		std::string shaderfolder_path(resource_directory + DIR_SLASH_CHAR + "shaders");
		std::string shader_type;

		bgfx::RendererType::Enum const render_type = bgfx::getRendererType();
		switch (render_type)
		{
			case bgfx::RendererType::Noop:
			case bgfx::RendererType::Direct3D9:  shader_type = "dx9";   break;
			case bgfx::RendererType::Direct3D11:
			case bgfx::RendererType::Direct3D12: shader_type = "dx11";  break;
			case bgfx::RendererType::Agc:
			case bgfx::RendererType::Gnm:        shader_type = "pssl";  break;
			case bgfx::RendererType::Metal:      shader_type = "metal"; break;
			case bgfx::RendererType::Nvn:        shader_type = "nvn";   break;
			case bgfx::RendererType::OpenGL:     shader_type = "glsl";  break;
			case bgfx::RendererType::OpenGLES:   shader_type = "essl";  break;
			case bgfx::RendererType::Vulkan:     shader_type = "spirv"; break;
			case bgfx::RendererType::WebGPU:     shader_type = "spirv"; break;

			case bgfx::RendererType::Count: DFW_ASSERT(false, "Failed to load shader, as the requested type is not supported. RenderAPI: {}", render_type); break;
		}
		
		std::string const vs_filename("vs_basic");
		std::string const fs_filename("fs_basic");
		std::string const shader_gen_fileextension(".bin");
		std::string const vs_filepath(shaderfolder_path + DIR_SLASH_CHAR + shader_type + DIR_SLASH_CHAR + vs_filename + shader_gen_fileextension);
		std::string const fs_filepath(shaderfolder_path + DIR_SLASH_CHAR + shader_type + DIR_SLASH_CHAR + fs_filename + shader_gen_fileextension);

		DFW_INFOLOG("Working Directory: {}", resource_directory);
		DFW_INFOLOG("VS File Path: {}", vs_filepath);
		DFW_INFOLOG("FS File Path: {}", fs_filepath);

		size_t buffer_size;
		uint8* const vs_filedata = DUtility::ReadBinaryFileIntoBuffer(buffer_size, vs_filepath);
		bgfx::Memory const* vs_mem = bgfx::copy(vs_filedata, static_cast<uint32>(buffer_size));
		uint8* const fs_filedata = DUtility::ReadBinaryFileIntoBuffer(buffer_size, fs_filepath);
		bgfx::Memory const* fs_mem = bgfx::copy(fs_filedata, static_cast<uint32>(buffer_size));

		bgfx::ShaderHandle const vs_handle = bgfx::createShader(vs_mem);
		bgfx::ShaderHandle const fs_handle = bgfx::createShader(fs_mem);

		m_program = bgfx::createProgram(vs_handle, fs_handle);

		delete fs_filedata;
		delete vs_filedata;
    }

	void RenderSystem::Terminate()
	{
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_program);
	}
    
    void RenderSystem::Update(DECS::Universe* const /*a_universe*/)
    {
		uint64_t state = 0
			| BGFX_STATE_WRITE_R
			| BGFX_STATE_WRITE_G
			| BGFX_STATE_WRITE_B
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			;

		// camera
		bx::Vec3 const at = { 0.0f, 0.0f,   0.0f };
		bx::Vec3 const eye = { 0.0f, 0.0f, -75.0f };

		DRender::ViewTarget const& view_target = *_view_target;
		// Set view and projection matrix for view 0.
		{
			float32 view[16];
			bx::mtxLookAt(view, eye, at);

			float32 proj[16];
			bx::mtxProj(proj, 60.0f, float32(1280) / float32(720), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(view_target, view, proj);

			// Set view 0 default viewport.
			bgfx::setViewRect(view_target, 0, 0, uint16_t(1280), uint16_t(720));
		}

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(view_target);

		// Submit 11x11 cubes.
		auto dt = CoreService::GetGameClock()->GetElapsedTimeInSeconds();
		for (uint32 yy = 0; yy < 12; ++yy)
		{
			for (uint32 xx = 0; xx < 12; ++xx)
			{
				float32 mtx[16];
				bx::mtxRotateXY(mtx, dt + xx * 0.21f, dt + yy * 0.37f);
				mtx[12] = -15.0f + float32(xx) * 3.0f;
				mtx[13] = -15.0f + float32(yy) * 3.0f;
				mtx[14] = 0.0f;

				// Set model matrix for rendering.
				bgfx::setTransform(mtx);

				// Set vertex and index buffer.
				bgfx::setVertexBuffer(0, m_vbh);
				bgfx::setIndexBuffer(m_ibh);

				// Set render states.
				bgfx::setState(state);

				// Submit primitive for rendering to view 0.
				bgfx::submit(view_target, m_program);
			}
		}
    }

} // End of namespace ~ DFW.
