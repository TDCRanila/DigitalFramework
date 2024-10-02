#include <DFW/GameWorld/Graphics/RenderSystem.h>

#include <DFW/GameWorld/Camera/CameraComponent.h>
#include <DFW/GameWorld/TransformComponent.h>
#include <DFW/GameWorld/Graphics/ModelComponent.h>

#include <DFW/Modules/ECS/Managers/EntityRegistry.h>

#include <DFW/Modules/Rendering/RenderModule.h>
#include <DFW/Modules/Rendering/ShaderLibrary.h>
#include <DFW/Modules/Rendering/UniformLibrary.h>
#include <DFW/Modules/Rendering/ViewTargetDirector.h>

#include <DFW/Modules/Rendering/ShaderProgram.h>
#include <DFW/Modules/Rendering/MeshData.h>
#include <DFW/Modules/Rendering/RenderTarget.h>
#include <DFW/Modules/Rendering/Uniform.h>

#include <DFW/CoreSystems/CoreServices.h>
#include <DFW/CoreSystems/Events/EventDispatcher.h>

#include <glm/gtc/type_ptr.hpp>

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace DFW
{
    void RenderSystem::Init(DECS::EntityRegistry& /*a_registry*/)
    {
		SharedPtr<DRender::RenderModule> render_module = CoreService::GetRenderModule();

		// View Target
		_view_target = render_module->GetViewDirector().AllocateViewTarget("rendersystem", DRender::ViewTargetInsertion::Front);

		// Shaders		
		_program_ptr = render_module->GetShaderLibrary().ConstructProgram("vs_basic", "fs_basic");

		// Uniforms
		_texture_sampler_uniform = render_module->GetUniformLibrary().CreateUniform("s_texture", DRender::UniformTypes::Sampler);

		// Register Callbacks.
		CoreService::GetAppEventHandler()->RegisterCallback<WindowResizeEvent, &BaseRenderSystem::OnWindowResizeEvent>(this);
		ECSEventHandler().RegisterCallback<CameraNewActiveEvent, &BaseRenderSystem::OnCameraNewActiveEvent>(this);
    }

	void RenderSystem::Terminate(DECS::EntityRegistry& /*a_registry*/)
	{
		// Uniforms
		CoreService::GetRenderModule()->GetUniformLibrary().DestroyUniform(*_texture_sampler_uniform.get());

		// Unregister Callbacks.
		CoreService::GetAppEventHandler()->UnregisterCallback<WindowResizeEvent, &BaseRenderSystem::OnWindowResizeEvent>(this);
		ECSEventHandler().UnregisterCallback<CameraNewActiveEvent, &BaseRenderSystem::OnCameraNewActiveEvent>(this);
	}
    
    void RenderSystem::Update(DECS::EntityRegistry& a_registry)
    {
		if (!_program_ptr)
			return;

		PrepareRenderTarget();
		PrepareViewTarget();

		constexpr uint64_t state = 0
			| BGFX_STATE_WRITE_R
			| BGFX_STATE_WRITE_G
			| BGFX_STATE_WRITE_B
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			;
		
		// Submit Primitives
		for (auto&& [entity, model, transform] : a_registry.ENTT().view<ModelComponent, TransformComponent>().each())
		{
			if (!model.is_visible)
				continue;

			for (DRender::SubMeshData const& submodel : model.mesh->submeshes)
			{
				bgfx::setTransform(glm::value_ptr(transform.GetWorldTransformMatrix()));
				bgfx::setVertexBuffer(0, submodel.vbh);
				bgfx::setIndexBuffer(submodel.ibh);
				
				if (!submodel.textures.empty())
				{
					SharedPtr<DRender::TextureData> const& texture = submodel.textures[0];
					bgfx::setTexture(texture->stage, _texture_sampler_uniform->handle, texture->handle, texture->flags | BGFX_SAMPLER_POINT);
				}

				bgfx::setState(state);
				bgfx::submit(_view_target->view_target_id, _program_ptr->shader_program_handle);
			}
		}
    }

} // End of namespace ~ DFW.
