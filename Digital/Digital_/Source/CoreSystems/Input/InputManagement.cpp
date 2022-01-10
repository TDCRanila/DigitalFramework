#include <CoreSystems/Input/InputManagement.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/CoreServices.h>

namespace DFW
{
	namespace DInput
	{
		InputManagementSystem::InputData::InputData()
			: _cursor_position(0.0f)
			, _cursor_position_old(0.0f)
			, _cursor_delta(0.0f)
			, _scroll_offset(0.0f)
			, _scroll_offset_old(0.0f)
			, _scroll_delta(0.0f)
		{
			_buffered_keys.reserve(16);
		}

		InputManagementSystem::InputManagementSystem()
			: _current_foccused_window_ptr(nullptr) 
			, _has_input_events_buffered(false)
			, _is_input_enabled(true)
		{
			_key_event_buffer.reserve(16);
			_dir_event_buffer.reserve(16);
		}

		void InputManagementSystem::InitInputManagement()
		{
			// Register Event Callbacks.
			CoreService::GetMainEventHandler()->RegisterCallback<WindowFocusEvent, &InputManagementSystem::OnWindowFocusEvent>(this);
		}

		void InputManagementSystem::TerminateInputManagement()
		{
			// Unregister Event Callbacks.
			CoreService::GetMainEventHandler()->UnregisterCallback<WindowFocusEvent, &InputManagementSystem::OnWindowFocusEvent>(this);
		}

		void InputManagementSystem::EnableInput()
		{
			_is_input_enabled = true;
		}

		void InputManagementSystem::DisableInput()
		{
			_is_input_enabled = false;
		}

		bool InputManagementSystem::IsKeyPressed(DKey a_key) const
		{
			return IsKeyPressedInternal(to_underlying(a_key));
		}

		bool InputManagementSystem::IsKeyRepeated(DKey a_key) const
		{
			return IsKeyRepeatedInternal(to_underlying(a_key));
		}

		bool InputManagementSystem::IsKeyDown(DKey a_key) const
		{
			return IsKeyDownInternal(to_underlying(a_key));
		}

		bool InputManagementSystem::IsKeyReleased(DKey a_key) const
		{
			return IsKeyReleasedInternal(to_underlying(a_key));
		}

		bool InputManagementSystem::IsKeyPressed(DMouse a_mouse_button) const
		{
			return IsKeyPressedInternal(to_underlying(a_mouse_button));
		}

		bool InputManagementSystem::IsKeyRepeated(DMouse a_mouse_button) const
		{
			return IsKeyRepeatedInternal(to_underlying(a_mouse_button));
		}

		bool InputManagementSystem::IsKeyDown(DMouse a_mouse_button) const
		{
			return IsKeyDownInternal(to_underlying(a_mouse_button));
		}

		bool InputManagementSystem::IsKeyReleased(DMouse a_mouse_button) const
		{
			return IsKeyReleasedInternal(to_underlying(a_mouse_button));
		}

		bool InputManagementSystem::IsKeyPressed(DGamePad a_gamepad_key) const
		{
			return IsKeyPressedInternal(to_underlying(a_gamepad_key));
		}

		bool InputManagementSystem::IsKeyRepeated(DGamePad a_gamepad_key) const
		{
			return IsKeyRepeatedInternal(to_underlying(a_gamepad_key));
		}

		bool InputManagementSystem::IsKeyDown(DGamePad a_gamepad_key) const
		{
			return IsKeyDownInternal(to_underlying(a_gamepad_key));
		}

		bool InputManagementSystem::IsKeyReleased(DGamePad a_gamepad_key) const
		{
			return IsKeyReleasedInternal(to_underlying(a_gamepad_key));
		}

		void InputManagementSystem::ProcessInputEvents()
		{
			ClearInputDataBuffers();

			if (!_has_input_events_buffered || !_is_input_enabled)
			{
				return;
			}

			InputData& data = _input_data;
			for (const KeyEvent& key_event : _key_event_buffer)
			{
				switch (key_event._event_type)
				{
				case (KeyEventType::KEYBOARD):
				case (KeyEventType::MOUSE):
				{
					// TODO Special Event Callbacks for Ctrl C & Ctrl V that send a message to Application
					DKey defined_key = static_cast<DKey>(key_event._key);
					DKeyAction defined_action = static_cast<DKeyAction>(key_event._action);

					data._keys[key_event._key] = defined_action;
					data._buffered_keys.emplace(to_underlying(defined_key), defined_action);

					break;
				}
				case(KeyEventType::CHARACTER):
				{
					data._buffered_characters.emplace_back(key_event._char);
					break;
				}
				case (KeyEventType::DEFAULT):
				default:
					break;
				}
			}

			_key_event_buffer.clear();

			for (const DirectionalEvent& dir_event : _dir_event_buffer)
			{
				switch (dir_event._event_type)
				{
				case (DirectionalEventType::CURSOR):
				{
					data._cursor_position_old.x = data._cursor_position.x;
					data._cursor_position_old.y = data._cursor_position.y;

					data._cursor_delta.x = dir_event._cursor_x_position - data._cursor_position.x;
					data._cursor_delta.y = dir_event._cursor_y_position - data._cursor_position.y;

					data._cursor_position.x = dir_event._cursor_x_position;
					data._cursor_position.y = dir_event._cursor_y_position;
					break;
				}
				case (DirectionalEventType::SCROLL):
				{
					data._scroll_delta.x = dir_event._scroll_x_offset;
					data._scroll_delta.y = dir_event._scroll_y_offset;

					data._scroll_offset_old.x = data._scroll_offset.x;
					data._scroll_offset_old.y = data._scroll_offset.y;

					data._scroll_offset.x += dir_event._scroll_x_offset;
					data._scroll_offset.y += dir_event._scroll_y_offset;
					break;
				}
				case (DirectionalEventType::DEFAULT):
				default:
					DFW_WARNLOG("InputManagement has received an event type that it cannot process.");
					break;
				}
			}

			_dir_event_buffer.clear();

			_has_input_events_buffered = false;
		}

		void InputManagementSystem::SendKeyEvent(DWindow::WindowID a_id, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
		{
			if (_is_input_enabled)
			{
				if ((a_key >= 0) && (a_key < 1024))
				{
					_has_input_events_buffered = true;
					_key_event_buffer.emplace_back(a_id, KeyEventType::KEYBOARD, a_key, a_scancode, a_action, a_modifier);
				}
			}
		}

		void InputManagementSystem::SendMouseEvent(DWindow::WindowID a_id, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
		{
			if (_is_input_enabled)
			{
				if ((a_key >= 0) && (a_key < 1024))
				{
					_has_input_events_buffered = true;
					_key_event_buffer.emplace_back(a_id, KeyEventType::MOUSE, a_key, a_scancode, a_action, a_modifier);
				}
			}
		}

		void InputManagementSystem::SendCharEvent(DWindow::WindowID a_id, uint16 a_char)
		{
			if (_is_input_enabled)
			{
				if (a_char >= 0)
				{
					_has_input_events_buffered = true;
					_key_event_buffer.emplace_back(a_id, KeyEventType::CHARACTER, a_char);
				}
			}
		}

		void InputManagementSystem::SendDirectionalEvent(DWindow::WindowID a_id, float32 a_x_offset, float32 a_y_offset)
		{
			if (_is_input_enabled)
			{
				_has_input_events_buffered = true;
				_dir_event_buffer.emplace_back(a_id, DirectionalEventType::CURSOR, a_x_offset, a_y_offset, 0.0f, 0.0f);
			}
		}

		void InputManagementSystem::SendScrollEvent(DWindow::WindowID a_id, float32 a_scroll_x_offset, float32 a_scroll_y_offset)
		{
			if (_is_input_enabled)
			{
				_has_input_events_buffered = true;
				_dir_event_buffer.emplace_back(a_id, DirectionalEventType::SCROLL, 0.0f, 0.0f, a_scroll_x_offset, a_scroll_y_offset);
			}
		}

		void InputManagementSystem::OnWindowFocusEvent(WindowFocusEvent const& a_event)
		{
			if (a_event.is_focussed)
				_current_foccused_window_ptr = CoreService::GetWindowSystem()->GetWindow(a_event.window_id);
		}

		bool InputManagementSystem::IsKeyPressedInternal(int32 a_key) const
		{
			const DKeyAction& key_action = _input_data._keys[a_key];
			return (key_action == DKeyAction::PRESSED);
		}

		bool InputManagementSystem::IsKeyRepeatedInternal(int32 a_key) const
		{
			const DKeyAction& key_action = _input_data._keys[a_key];
			return (key_action == DKeyAction::REPEATED);
		}

		bool InputManagementSystem::IsKeyDownInternal(int32 a_key) const
		{
			const DKeyAction& key_action = _input_data._keys[a_key];
			return (key_action == DKeyAction::PRESSED) || (key_action == DKeyAction::REPEATED);
		}

		bool InputManagementSystem::IsKeyReleasedInternal(int32 a_key) const
		{
			InputData const& data = _input_data;
			if (_input_data._buffered_keys.empty())
				return false;

			auto it_key = data._buffered_keys.find(a_key);
			if (it_key != data._buffered_keys.end())
			{
				const DKeyAction& key_action = data._keys[a_key];
				return (key_action == DKeyAction::RELEASED);
			}
			return false;
		}

		void InputManagementSystem::ClearInputDataBuffers()
		{
			_input_data._buffered_keys.clear();
			_input_data._buffered_characters.clear();
		}

		InputManagementSystem::KeyEvent::KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, int32 a_key, uint16 a_char, int32 a_scancode, int32 a_action, int32 a_modifier)
			: _user_id(a_id)
			, _event_type(a_event_type)
			, _key(a_key)
			, _char(a_char)
			, _scancode(a_scancode)
			, _action(a_action)
			, _modifier(a_modifier)
		{
		}

		InputManagementSystem::KeyEvent::KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
			: _user_id(a_id)
			, _event_type(a_event_type)
			, _key(a_key)
			, _scancode(a_scancode)
			, _action(a_action)
			, _modifier(a_modifier)
		{
		}

		InputManagementSystem::KeyEvent::KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, uint16 a_char)
			: _user_id(a_id)
			, _event_type(a_event_type)
			, _key(to_underlying(DKey::UNDEFINED))
			, _char(a_char)
		{
		}

		InputManagementSystem::DirectionalEvent::DirectionalEvent(DWindow::WindowID a_id, DirectionalEventType a_event_type,
			float32 a_cursor_x_pos, float32 a_cursor_y_pos, float32 a_scroll_x_offset, float32 a_scroll_y_offset)
			: _user_id(a_id)
			, _event_type(a_event_type)
			, _cursor_x_position(a_cursor_x_pos)
			, _cursor_y_position(a_cursor_y_pos)
			, _scroll_x_offset(a_scroll_x_offset)
			, _scroll_y_offset(a_scroll_y_offset)
		{
		}

	} // End of namespace ~ DInput.

} // End of namespace ~ DFW.
