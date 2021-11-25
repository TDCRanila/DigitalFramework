#pragma once

namespace DFW
{
	// FW Declare.
	class ApplicationInstance;

	class ImGuiLayer final
	{
	public:
		~ImGuiLayer() = default;

		void InitImGuiLayer();
		void TerminateImGuiLayer();

		void BeginFrame(float32 const a_delta_time);
		void EndFrame();

	protected:
		friend ApplicationInstance;

		ImGuiLayer() = default;
	
	private:
		void SetupStyle();

		void Debug_DrawViewportRenderingInformation() const;
	};

} // End of namespace ~ DFW.
