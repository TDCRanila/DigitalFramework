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
	
	public:
		void Debug_DisplayViewportRenderInformation() const;

	private:
		void SetupStyle();

	};

} // End of namespace ~ DFW.
