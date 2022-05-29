#include <GameWorld/Graphics/BaseRenderSystem.h>

#include <GameWorld/Camera/CameraSystem.h>

#include <Modules/ECS/Managers/ECSystemManager.h>
#include <Modules/Rendering/RenderModule.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/Window/WindowManagement.h>

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
		if (!CoreService::GetRenderModule()->render_target_director.IsRenderTargetValid(*a_render_target))
		{
			DFW_ERRORLOG("Attempting to set the RenderTarget for RenderSystem, but the render target is invalid.");
			DFW_ASSERT(false, "Attempting to set the RenderTarget for RenderSystem, but the render target is invalid.");
			return;
		}

		_render_target = a_render_target;
	}

	void BaseRenderSystem::RenderToDefault()
	{
		_render_target = nullptr;
	}

	void BaseRenderSystem::OnWindowResizeEvent(WindowResizeEvent const& a_window_event)
	{
		window_width = a_window_event.new_width;
		window_height = a_window_event.new_height;
	}

	void BaseRenderSystem::OnCameraNewActiveEvent(CameraNewActiveEvent const& a_camera_event)
	{
		_rendering_camera = SystemManager().GetSystem<CameraSystem>()->GetCamera(a_camera_event.camera_identifier);
	}

} // End of namespace ~ DFW.
