#include <CoreSystems/Systems/RenderSystem.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Memory.h>
#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/Rendering/ViewTargetDirector.h>
#include <Modules/Rendering/RenderModule.h>
#include <Modules/Rendering/ShaderLibrary.h>
#include <Modules/Rendering/ShaderProgram.h>

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
DFW::SharedPtr<DFW::DRender::ShaderProgram> _program_ptr;

namespace DFW
{
	RenderSystem::RenderSystem()
		: window_width(DWindow::DFW_DEFAULT_WINDOW_WIDTH)
		, window_height(DWindow::DFW_DEFAULT_WINDOW_HEIGHT)
	{
	}

    void RenderSystem::Init()
    {
		// View Target
		_view_target = CoreService::GetRenderModule()->view_director.AllocateViewTarget("rendersystem");

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
		_program_ptr = CoreService::GetRenderModule()->shader_library.ConstructProgram("vs_basic", "fs_basic");

		// Register Callbacks.
		CoreService::GetMainEventHandler()->RegisterCallback<WindowResizeEvent, &RenderSystem::OnWindowResizeEvent>(this);
    }

	void RenderSystem::Terminate()
	{
		// Unregister Callbacks.
		CoreService::GetMainEventHandler()->UnregisterCallback<WindowResizeEvent, &RenderSystem::OnWindowResizeEvent>(this);

		bgfx::destroy(m_vbh);
		bgfx::destroy(m_ibh);
	}
    
    void RenderSystem::Update(DECS::Universe* const /*a_universe*/)
    {
		if (!_program_ptr)
			return;

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

		DRender::ViewTarget const& view_target = *_view_target;
		bgfx::touch(view_target);
		
		{
			bx::Vec3 const at	= { 0.0f, 0.0f,   0.0f };
			bx::Vec3 const eye	= { 0.0f, 0.0f, -75.0f };
			
			float32 view[16];
			bx::mtxLookAt(view, eye, at);

			float32 proj[16];
			bx::mtxProj(proj, 60.0f, float32(window_width) / float32(window_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(view_target, view, proj);

			bgfx::setViewRect(view_target, 0, 0, static_cast<uint16>(window_width), static_cast<uint16>(window_height));
		}

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

				bgfx::setTransform(mtx);

				bgfx::setVertexBuffer(0, m_vbh);
				bgfx::setIndexBuffer(m_ibh);

				bgfx::setState(state);

				bgfx::submit(view_target, _program_ptr->shader_program_handle);
			}
		}
    }

	void RenderSystem::OnWindowResizeEvent(WindowResizeEvent const& a_window_event)
	{
		window_width	= a_window_event.new_width;
		window_height	= a_window_event.new_height;
	}

} // End of namespace ~ DFW.
