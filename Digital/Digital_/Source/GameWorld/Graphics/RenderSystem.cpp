#include <GameWorld/Graphics/RenderSystem.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/GameClock.h>
#include <CoreSystems/Memory.h>
#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <GameWorld/Camera/CameraComponent.h>
#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/TransformComponent.h>
#include <GameWorld/Graphics/ModelComponent.h>

#include <CoreSystems/Events/EventDispatcher.h>
#include <Modules/ECS/Managers/ECSystemManager.h>
#include <Modules/ECS/Objects/ECSUniverse.h>

#include <Modules/Rendering/RenderModule.h>
#include <Modules/Rendering/ShaderProgram.h>
#include <Modules/Rendering/ModelData.h>

#include <glm/gtc/type_ptr.hpp>

#include <bgfx/bgfx.h>
#include <bx/math.h>

DFW::SharedPtr<DFW::DRender::ShaderProgram> _program_ptr;

namespace DFW
{
	RenderSystem::RenderSystem()
		: _rendering_camera(nullptr)
		, window_width(DWindow::DFW_DEFAULT_WINDOW_WIDTH)
		, window_height(DWindow::DFW_DEFAULT_WINDOW_HEIGHT)
	{
	}

    void RenderSystem::Init()
    {
		DRender::RenderModule* render_module(CoreService::GetRenderModule());

		// View Target
		_view_target = render_module->view_director.AllocateViewTarget("rendersystem");

		// Shaders		
		_program_ptr = render_module->shader_library.ConstructProgram("vs_basic", "fs_basic");

		// Uniforms
		_texture_sampler_uniform = render_module->uniform_library.CreateUniform("s_texture", DRender::UniformTypes::Sampler);

		// Register Callbacks.
		CoreService::GetMainEventHandler()->RegisterCallback<WindowResizeEvent, &RenderSystem::OnWindowResizeEvent>(this);
		ECSEventHandler().RegisterCallback<CameraNewActiveEvent, &RenderSystem::OnCameraNewActiveEvent>(this);
    }

	void RenderSystem::Terminate()
	{
		// Uniforms
		CoreService::GetRenderModule()->uniform_library.DestroyUniform(*_texture_sampler_uniform.get());

		// Unregister Callbacks.
		CoreService::GetMainEventHandler()->UnregisterCallback<WindowResizeEvent, &RenderSystem::OnWindowResizeEvent>(this);
		ECSEventHandler().UnregisterCallback<CameraNewActiveEvent, &RenderSystem::OnCameraNewActiveEvent>(this);
	}
    
    void RenderSystem::Update(DECS::Universe& a_universe)
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

		// Clear Target.
		DRender::ViewTarget const& view_target = *_view_target;
		bgfx::touch(view_target);
		
		// Camera Setup.
		if (_rendering_camera)
		{
			bgfx::setViewTransform(view_target, glm::value_ptr(_rendering_camera->view), glm::value_ptr(_rendering_camera->projection));
			bgfx::setViewRect(view_target, 0, 0, static_cast<uint16>(window_width), static_cast<uint16>(window_height));
		}
		else
		{
			// No camera present, create a "camera" at world origin.
			float32 view[16];
			bx::Vec3 const at	= { 0.0f, 0.0f, 1.0f };
			bx::Vec3 const eye	= { 0.0f, 0.0f, 0.0f };
			bx::mtxLookAt(view, eye, at);

			float32 proj[16];
			bx::mtxProj(proj, DFW_DEFAULT_CAMERA_FOV, float32(window_width) / float32(window_height), 0.1f, 1000.0f, bgfx::getCaps()->homogeneousDepth);

			bgfx::setViewTransform(view_target, view, proj);
			bgfx::setViewRect(view_target, 0, 0, static_cast<uint16>(window_width), static_cast<uint16>(window_height));
		}

		// Submit Primitives
		for (auto&& [entity, model, transform] : a_universe.registry.view<ModelComponent, TransformComponent>().each())
		{
			if (!model.is_visible)
				continue;

			for (DRender::SubMeshData const& submodel : model.model->submeshes)
			{
				bgfx::setTransform(glm::value_ptr(transform.Transform()));
				bgfx::setVertexBuffer(0, submodel.vbh);
				bgfx::setIndexBuffer(submodel.ibh);
				
				if (!submodel.textures.empty())
				{
					SharedPtr<DRender::TextureData> const& texture = submodel.textures[0];
					bgfx::setTexture(texture->stage, _texture_sampler_uniform->handle, texture->handle);
				}

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

	void RenderSystem::OnCameraNewActiveEvent(CameraNewActiveEvent const& a_camera_event)
	{
		_rendering_camera = SystemManager().GetSystem<CameraSystem>()->GetCamera(a_camera_event.camera_identifier);
	}

} // End of namespace ~ DFW.
