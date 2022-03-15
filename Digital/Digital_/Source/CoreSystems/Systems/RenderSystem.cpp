#include <CoreSystems/Systems/RenderSystem.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Memory.h>
#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <CoreSystems/Components/ModelComponent.h>
#include <CoreSystems/Components/TransformComponent.h>

#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <Modules/Rendering/ViewTargetDirector.h>
#include <Modules/Rendering/RenderModule.h>
#include <Modules/Rendering/ShaderLibrary.h>
#include <Modules/Rendering/ShaderProgram.h>
#include <Modules/Rendering/ModelData.h>

#include <Utility/FileSystemUtility.h>

#include <bgfx/bgfx.h>
#include <bx/math.h>

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

		// Shaders		
		_program_ptr = CoreService::GetRenderModule()->shader_library.ConstructProgram("vs_basic", "fs_basic");

		// Register Callbacks.
		CoreService::GetMainEventHandler()->RegisterCallback<WindowResizeEvent, &RenderSystem::OnWindowResizeEvent>(this);
    }

	void RenderSystem::Terminate()
	{
		// Unregister Callbacks.
		CoreService::GetMainEventHandler()->UnregisterCallback<WindowResizeEvent, &RenderSystem::OnWindowResizeEvent>(this);
	}
    
    void RenderSystem::Update(DECS::Universe* const a_universe)
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
			bx::Vec3 const eye	= { 0.0f, 0.0f, -5.0f };
			
			float32 view[16];
			bx::mtxLookAt(view, eye, at);

			float32 proj[16];
			bx::mtxProj(proj, 60.0f, float32(window_width) / float32(window_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(view_target, view, proj);

			bgfx::setViewRect(view_target, 0, 0, static_cast<uint16>(window_width), static_cast<uint16>(window_height));
		}

		auto view = a_universe->_registry.view<TransformComponent, ModelComponent>();
		for (DECS::EntityHandle const& entity : view)
		{
			ModelComponent const& model = view.get<ModelComponent>(entity);
			if (!model.is_visible)
				continue;

			TransformComponent const& transform	= view.get<TransformComponent>(entity);
			bgfx::setTransform(&transform.Transform()[0][0]);

			bgfx::setVertexBuffer(0, model.data->submodels[0].vbh);
			bgfx::setIndexBuffer(model.data->submodels[0].ibh);

			bgfx::setState(state);

			bgfx::submit(view_target, _program_ptr->shader_program_handle);
		}
    }

	void RenderSystem::OnWindowResizeEvent(WindowResizeEvent const& a_window_event)
	{
		window_width	= a_window_event.new_width;
		window_height	= a_window_event.new_height;
	}

} // End of namespace ~ DFW.
