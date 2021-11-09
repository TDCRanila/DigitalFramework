#pragma once

#include <CoreSystems/Window/WindowManagement.h>

namespace DCore
{
	// FW Declare.
	class ApplicationInstance;

	class ImGuiLayer final
	{
	public:
		~ImGuiLayer() = default;

		void InitImGuiLayer(WindowInstance const& a_main_window);
		void TerminateImGuiLayer();

		void BeginFrame(float32 const a_delta_time);
		void EndFrame();

	protected:
		friend ApplicationInstance;

		void SetMainImGuiWindowSize(WindowResizeEvent& const a_event);

		ImGuiLayer() = default;
	
	private:
		void SetupStyle();

		void Debug_DrawViewportRenderingInformation() const;
	};

} // End of namespace ~ DCore.
