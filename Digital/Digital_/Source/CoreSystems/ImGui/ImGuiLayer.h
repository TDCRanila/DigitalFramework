#pragma once

#include <CoreSystems/InputManagement.h>
#include <CoreSystems/Window/WindowManagement.h>

namespace bgfx
{
	typedef uint16_t ViewId;
} // End of namespace ~ bgfx.

namespace DCore
{
	class ImGuiLayer
	{
	public:
		ImGuiLayer() = default;

		void InitImGuiLayer(const WindowInstance& a_window_instance);
		void TerminateImGuiLayer();

		void BeginFrame(const DCore::InputData& a_input_data, const DCore::WindowDimension& a_window_dimension);
		void EndFrame();
	private:

		void SetupStyle();
	};

} // End of namespace ~ DCore.
