#include <GameWorld/Graphics/BaseRenderSystem.h>

#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Camera/CameraComponent.h>

#include <Modules/ECS/Managers/SystemManager.h>
#include <Modules/Rendering/RenderModule.h>
#include <Modules/Rendering/RenderTargetDirector.h>
#include <Modules/Rendering/ViewTargetDirector.h>
#include <Modules/Rendering/RenderTarget.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <glm/gtc/type_ptr.hpp>

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace DFW
{
	BaseRenderSystem::BaseRenderSystem()
		: _view_target(nullptr)
		, _render_target(nullptr)
		, _program_ptr(nullptr)
		, _rendering_camera(nullptr)
		, window_width(DWindow::DFW_DEFAULT_WINDOW_WIDTH)
		, window_height(DWindow::DFW_DEFAULT_WINDOW_HEIGHT)
	{
	}

	void BaseRenderSystem::RenderToRenderTarget(SharedPtr<DRender::RenderTarget const> const& a_render_target)
	{
		if (!CoreService::GetRenderModule()->GetRenderTargetDirector().IsRenderTargetValid(*a_render_target))
		{
			DFW_ERRORLOG("Attempting to set the RenderTarget for RenderSystem, but the render target is invalid.");
			DFW_ASSERT(false && "Attempting to set the RenderTarget for RenderSystem, but the render target is invalid.");
			return;
		}

		_render_target = a_render_target;
	}

	void BaseRenderSystem::RenderToDefault()
	{
		_render_target = nullptr;
	}

	void BaseRenderSystem::PrepareViewTarget()
	{
		DRender::ViewTarget const& view_target = *_view_target;
		bgfx::touch(view_target.view_target_id);

		if (_rendering_camera)
		{
			bgfx::setViewTransform(view_target, glm::value_ptr(_rendering_camera->view), glm::value_ptr(_rendering_camera->projection));
			bgfx::setViewRect(view_target, 0, 0, static_cast<uint16>(window_width), static_cast<uint16>(window_height));
		}
		else
		{
			// No camera present, create a "camera" at world origin.
			float32 view[16];
			bx::Vec3 const at = { 0.0f, 0.0f, 1.0f };
			bx::Vec3 const eye = { 0.0f, 0.0f, 0.0f };
			bx::mtxLookAt(view, eye, at);

			float32 proj[16];
			bx::mtxProj(proj, DFW_DEFAULT_CAMERA_FOV, float32(window_width) / float32(window_height), 0.1f, 1000.0f, bgfx::getCaps()->homogeneousDepth);

			bgfx::setViewTransform(view_target, view, proj);
			bgfx::setViewRect(view_target, 0, 0, static_cast<uint16>(window_width), static_cast<uint16>(window_height));
		}
	}

	void BaseRenderSystem::PrepareRenderTarget()
	{
		// If nullptr, RenderSystem will be drawing to the default framebuffer.
		if (_render_target)
		{
			bgfx::setViewFrameBuffer(_view_target->view_target_id, _render_target->fbh);
		}
	}

	void BaseRenderSystem::OnWindowResizeEvent(WindowResizeEvent const& a_window_event)
	{
		window_width = a_window_event.new_width;
		window_height = a_window_event.new_height;
	}

	void BaseRenderSystem::OnCameraNewActiveEvent(CameraNewActiveEvent const& a_camera_event)
	{
		_rendering_camera = SystemManager().GetSystem<CameraSystem>()->GetCamera(a_camera_event.camera_name);
	}

} // End of namespace ~ DFW.
