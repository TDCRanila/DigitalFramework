#pragma once

#include <CoreSystems/Window/WindowID.h>
#include <CoreSystems/Input/InputKeys.h>
#include <Utility/TemplateUtility.h>

#include <glm/glm.hpp>

#include <array>
#include <functional>

namespace DFW
{
	// FW Declare.
	namespace DWindow
	{
		class WindowManagementSystem;
		struct WindowInstance;
	} // End of namespace ~ DWindow.

	class ApplicationInstance;

	namespace DInput
	{
		struct InputData
		{
			InputData();

			std::array<DKeyAction, 1024>			_keys{ DKeyAction::UNDEFINED };
			std::unordered_map<int32, DKeyAction>	_buffered_keys;
			std::vector<uint32>						_buffered_characters;

			glm::vec2 _cursor_position;
			glm::vec2 _cursor_position_old;
			glm::vec2 _cursor_delta;

			glm::vec2 _scroll_offset;
			glm::vec2 _scroll_offset_old;
			glm::vec2 _scroll_delta;

		};

		class InputManagementSystem
		{
		private:
			enum class KeyEventType;
			enum class DirectionalEventType;

		public:

			~InputManagementSystem();

			void EnableInput();
			void DisableInput();

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

			bool HasAnyKeyBeenPressed() const;
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

		protected:

			friend ApplicationInstance;

			InputManagementSystem();

			void ProcessInputEvents();
			void PollClipboardInput();

			friend DWindow::WindowManagementSystem;

			void RegisterWindow(DWindow::WindowInstance* a_window);
			void UnregisterWindow(DWindow::WindowInstance* a_window);

		private:

			enum class KeyEventType
			{
				DEFAULT = 0,
				KEYBOARD = 1,
				MOUSE = 2,
				CHARACTER = 3,
			};

			enum class DirectionalEventType
			{
				DEFAULT = 0,
				CURSOR = 1,
				SCROLL = 2,
			};

			struct KeyEvent
			{
				KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, int32 a_key, uint16 a_char, int32 a_scancode, int32 a_action, int32 a_modifier);
				KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier);
				KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, uint16 a_char);

				KeyEventType	_event_type;
				DWindow::WindowID		_user_id;

				int32			_key;
				int32			_scancode;
				int32			_action;
				int32			_modifier;
				uint32			_char;
			};

			struct DirectionalEvent
			{
				DirectionalEvent(DWindow::WindowID a_id, DirectionalEventType a_event_type, float32 a_cursor_x_pos, float32 a_cursor_y_pos, float32 a_scroll_x_offset, float32 a_scroll_y_offset);

				DirectionalEventType _event_type;
				DWindow::WindowID _user_id;

				float32 _cursor_x_position;
				float32 _cursor_y_position;
				float32 _scroll_x_offset;
				float32 _scroll_y_offset;
			};

			bool IsKeyPressedInternal(int32 a_key) const;
			bool IsKeyRepeatedInternal(int32 a_key) const;
			bool IsKeyDownInternal(int32 a_key) const;
			bool IsKeyReleasedInternal(int32 a_key) const;

			void ClearInputDataBuffers();

			std::unordered_map<DWindow::WindowID, InputData> _input_data_storage;

			std::vector<KeyEvent>			_key_event_buffer;
			std::vector<DirectionalEvent>	_dir_event_buffer;

			bool _has_input_events_buffered;
			bool _is_input_enabled;

		};

	} // End of namespace ~ DInput.

} // End of namespace ~ DFW.
