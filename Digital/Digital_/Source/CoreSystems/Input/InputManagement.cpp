#include <CoreSystems/Input/InputManagement.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
	namespace DInput
	{
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
			CoreService::GetAppEventHandler()->RegisterCallback<WindowFocusEvent, &InputManagementSystem::OnWindowFocusEvent>(this);
			CoreService::GetAppEventHandler()->RegisterCallback<InputMouseCursorReleasedEvent, &InputManagementSystem::OnMouseCursorReleasedEvent>(this);
		}

		void InputManagementSystem::TerminateInputManagement()
		{
			// Unregister Event Callbacks.
			CoreService::GetAppEventHandler()->UnregisterCallback<WindowFocusEvent, &InputManagementSystem::OnWindowFocusEvent>(this);
			CoreService::GetAppEventHandler()->UnregisterCallback<InputMouseCursorReleasedEvent, &InputManagementSystem::OnMouseCursorReleasedEvent>(this);
		}

		void InputManagementSystem::ProcessInputEvents()
		{
			_input_data.ClearBuffers();

			if (!_has_input_events_buffered || !_is_input_enabled)
			{
				return;
			}

			InputData& data = _input_data;
			data.has_buffered_data = true;
			for (KeyEvent const& key_event : _key_event_buffer)
			{
				switch (key_event.event_type)
				{
				case (KeyEventType::KEYBOARD):
				{
					DKey const defined_key = static_cast<DKey>(key_event.key);
					DKeyAction const defined_action = static_cast<DKeyAction>(key_event.action);

					data.keys[key_event.key] = defined_action;
					data.buffered_keys.emplace(to_underlying(defined_key), defined_action);

					data.has_received_key_input_this_frame = true;
					break;
				}
				case (KeyEventType::MOUSE):
				{
					DKey const defined_key = static_cast<DKey>(key_event.key);
					DKeyAction const defined_action = static_cast<DKeyAction>(key_event.action);

					data.keys[key_event.key] = defined_action;
					data.buffered_keys.emplace(to_underlying(defined_key), defined_action);

					data.has_received_mouse_input_this_frame = true;

					break;
				}
				case(KeyEventType::CHARACTER):
				{
					data.buffered_characters.emplace_back(key_event.character);
					data.has_received_key_input_this_frame = true;
					break;
				}
				case (KeyEventType::DEFAULT):
				default:
					break;
				}
			}

			for (const DirectionalEvent& dir_event : _dir_event_buffer)
			{
				switch (dir_event.event_type)
				{
				case (DirectionalEventType::CURSOR):
				{
					data.cursor_position_old.x = data.cursor_position.x;
					data.cursor_position_old.y = data.cursor_position.y;

					data.cursor_delta.x = dir_event.cursor_x_position - data.cursor_position.x;
					data.cursor_delta.y = dir_event.cursor_y_position - data.cursor_position.y;

					data.cursor_position.x = dir_event.cursor_x_position;
					data.cursor_position.y = dir_event.cursor_y_position;

					data.has_mouse_moved_this_frame = true;

					break;
				}
				case (DirectionalEventType::SCROLL):
				{
					data.scroll_delta.x = dir_event.scroll_x_offset;
					data.scroll_delta.y = dir_event.scroll_y_offset;

					data.scroll_offset_old.x = data.scroll_offset.x;
					data.scroll_offset_old.y = data.scroll_offset.y;

					data.scroll_offset.x += dir_event.scroll_x_offset;
					data.scroll_offset.y += dir_event.scroll_y_offset;

					data.has_received_scroll_input_this_frame = true;

					break;
				}
				case (DirectionalEventType::DEFAULT):
				default:
					DFW_WARNLOG("InputManagement has received an event type that it cannot process.");
					break;
				}
			}

			_key_event_buffer.clear();
			_dir_event_buffer.clear();

			_has_input_events_buffered = false;
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

		glm::vec2 InputManagementSystem::GetMousePos() const
		{
			return _input_data.cursor_position;
		}

		glm::vec2 InputManagementSystem::GetMousePosDelta() const
		{
			return _input_data.cursor_delta;
		}

		glm::vec2 InputManagementSystem::GetMouseScrollOffset() const
		{
			return _input_data.scroll_offset;
		}

		glm::vec2 InputManagementSystem::GetMouseScrollDelta() const
		{
			return _input_data.scroll_delta;
		}

		bool InputManagementSystem::HasAnyKeyBeenPressed() const
		{
			return _input_data.has_received_key_input_this_frame;
		}

		bool InputManagementSystem::HasAnyMouseButtonBeenPressed() const
		{
			return _input_data.has_received_mouse_input_this_frame;
		}

		bool InputManagementSystem::HasCursorMoved() const
		{
			return _input_data.has_mouse_moved_this_frame;
		}

		bool InputManagementSystem::HasScrolled() const
		{
			return _input_data.has_received_scroll_input_this_frame;
		}

		void InputManagementSystem::SendKeyEvent(DWindow::WindowID a_id, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
		{
			if (_is_input_enabled && (a_key >= 0) && (a_key < 1024))
			{
				_has_input_events_buffered = true;
				_key_event_buffer.emplace_back(a_id, KeyEventType::KEYBOARD, a_key, a_scancode, a_action, a_modifier);
			}
		}

		void InputManagementSystem::SendMouseEvent(DWindow::WindowID a_id, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
		{
			if (_is_input_enabled && (a_key >= 0) && (a_key < 1024))
			{
				_has_input_events_buffered = true;
				_key_event_buffer.emplace_back(a_id, KeyEventType::MOUSE, a_key, a_scancode, a_action, a_modifier);
			}
		}

		void InputManagementSystem::SendCharEvent(DWindow::WindowID a_id, uint16 a_char)
		{
			if (_is_input_enabled && (a_char >= 0))
			{
				_has_input_events_buffered = true;
				_key_event_buffer.emplace_back(a_id, KeyEventType::CHARACTER, a_char);
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
				_current_foccused_window_ptr = CoreService::GetWindowManagement()->GetWindow(a_event.window_id);
		}

		void InputManagementSystem::OnMouseCursorReleasedEvent(InputMouseCursorReleasedEvent const& a_event)
		{
			glm::vec2 const reset_cursor_position(a_event._new_mouse_pos_x, a_event._new_mouse_pos_y);
			_input_data.cursor_position		= reset_cursor_position;
			_input_data.cursor_position_old = reset_cursor_position;
			_input_data.cursor_delta		= glm::vec2(0.0f);
		}

		bool InputManagementSystem::IsKeyPressedInternal(int16 a_key) const
		{
			if (_input_data.buffered_keys.empty())
				return false;

			if (auto const& it_key = _input_data.buffered_keys.find(a_key); it_key != _input_data.buffered_keys.end())
				return (it_key->second == DKeyAction::PRESSED);
			else
				return false;
		}

		bool InputManagementSystem::IsKeyRepeatedInternal(int16 a_key) const
		{
			DKeyAction const& key_action = _input_data.keys[a_key];
			return (key_action == DKeyAction::REPEATED);
		}

		bool InputManagementSystem::IsKeyDownInternal(int16 a_key) const
		{
			DKeyAction const& key_action = _input_data.keys[a_key];
			return (key_action == DKeyAction::PRESSED) || (key_action == DKeyAction::REPEATED);
		}

		bool InputManagementSystem::IsKeyReleasedInternal(int16 a_key) const
		{
			if (_input_data.buffered_keys.empty())
				return false;

			if (auto const& it_key = _input_data.buffered_keys.find(a_key); it_key != _input_data.buffered_keys.end())
				return (it_key->second == DKeyAction::RELEASED);
			else
				return false;
		}
	} // End of namespace ~ DInput.

} // End of namespace ~ DFW.
