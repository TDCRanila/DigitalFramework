#pragma once

namespace DFW
{
	class ImGuiLayer final
	{
	public:
		ImGuiLayer() = default;
		~ImGuiLayer() = default;

		void Init();
		void Terminate();

		void BeginFrame(float32 const a_delta_time);
		void EndFrame();
	
	private:
		void SetupStyle();

		void Debug_DrawViewportRenderingInformation() const;
	};

} // End of namespace ~ DFW.
