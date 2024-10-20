#pragma once

#include <DFW/CoreSystems/ApplicationEvents.h>

#include <DFW/CoreSystems/Input/InputKeys.h>
#include <DFW/CoreSystems/Input/InputData.h>
#include <DFW/CoreSystems/Input/RawInputEvents.h>

#include <DFW/CoreSystems/Memory.h>

#include <DFW/CoreSystems/Window/WindowData.h>
#include <DFW/CoreSystems/Window/WindowID.h>

#include <glm/glm.hpp>

namespace DFW
{
	namespace DInput
	{
		class InputManagementSystem
		{
		public:
			InputManagementSystem();
			~InputManagementSystem() = default;

			void Init();
			void Terminate();

			void ProcessInputEvents();

			void EnableInput();
			void DisableInput();

		public:
			bool IsKeyPressed(DKey a_key) const;
			bool IsKeyRepeated(DKey a_key) const;
			bool IsKeyDown(DKey a_key) const;
			bool IsKeyReleased(DKey a_key) const;

			bool IsKeyPressed(DMouse a_mouse_button) const;
			bool IsKeyRepeated(DMouse a_mouse_button) const;
			bool IsKeyDown(DMouse a_mouse_button) const;
			bool IsKeyReleased(DMouse a_mouse_button) const;

			bool IsKeyPressed(DGamePad a_gamepad_key) const;
			bool IsKeyRepeated(DGamePad a_gamepad_key) const;
			bool IsKeyDown(DGamePad a_gamepad_key) const;
			bool IsKeyReleased(DGamePad a_gamepad_key) const;

			glm::vec2 GetMousePos() const; // top-left[0,0];bottom-right[window_size.x,window_size.y]
			glm::vec2 GetMousePosDelta() const;

			glm::vec2 GetMouseScrollOffset() const;
			glm::vec2 GetMouseScrollDelta() const;

			bool HasAnyKeyBeenPressed() const;
			bool HasAnyMouseButtonBeenPressed() const;
			bool HasCursorMoved() const;
			bool HasScrolled() const;

			void RegisterInputEvent(std::string a_key_event, std::function<void> a_function, std::string a_input_profile = "default");

			void DefaultInputProfile();
			void ChangeActiveInputProfile(std::string a_new_input_profile);

			void SendKeyEvent(DWindow::WindowID a_id, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier);
			void SendMouseEvent(DWindow::WindowID a_id, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier);
			void SendCharEvent(DWindow::WindowID a_id, uint16 a_char);
			void SendDirectionalEvent(DWindow::WindowID a_id, float32 a_x_offset, float32 a_y_offset);
			void SendScrollEvent(DWindow::WindowID a_id, float32 a_scroll_x_offset, float32 a_scroll_y_offset);

		private:
			void OnWindowFocusEvent(WindowFocusEvent const& a_event);
			void OnMouseCursorReleasedEvent(InputMouseCursorReleasedEvent const& a_event);

			bool IsKeyPressedInternal(int16 a_key) const;
			bool IsKeyRepeatedInternal(int16 a_key) const;
			bool IsKeyDownInternal(int16 a_key) const;
			bool IsKeyReleasedInternal(int16 a_key) const;

		private:
			std::vector<KeyEvent>			_key_event_buffer;
			std::vector<DirectionalEvent>	_dir_event_buffer;

			SharedPtr<DWindow::WindowInstance> _current_foccused_window_ptr;
			InputData _input_data;

			bool _has_input_events_buffered;
			bool _is_input_enabled;

		};

	} // End of namespace ~ DInput.

} // End of namespace ~ DFW.
