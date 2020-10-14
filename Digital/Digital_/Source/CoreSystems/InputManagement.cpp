#include <CoreSystems/InputManagement.h>

#include <CoreSystems/ApplicationInstance.h>

namespace DCore
{
	InputData::InputData() 
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
		:	_has_input_events_buffered(false)
		,	_is_input_enabled(true)
	{
		_key_event_buffer.reserve(16);
		_dir_event_buffer.reserve(16);
	}

	InputManagementSystem::~InputManagementSystem()
	{
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
		return IsKeyPressed(to_underlying(a_key));
	}

	bool InputManagementSystem::IsKeyDown(DKey a_key) const
	{
		return IsKeyDown(to_underlying(a_key));
	}

	bool InputManagementSystem::IsKeyReleased(DKey a_key) const
	{
		return IsKeyReleased(to_underlying(a_key));
	}

	bool InputManagementSystem::IsKeyPressed(DMouse a_mouse_button) const
	{
		return IsKeyPressed(to_underlying(a_mouse_button));
	}

	bool InputManagementSystem::IsKeyDown(DMouse a_mouse_button) const
	{
		return IsKeyDown(to_underlying(a_mouse_button));
	}

	bool InputManagementSystem::IsKeyReleased(DMouse a_mouse_button) const
	{
		return IsKeyReleased(to_underlying(a_mouse_button));
	}

	bool InputManagementSystem::IsKeyPressed(DJoy a_joykey_key) const
	{
		return IsKeyPressed(to_underlying(a_joykey_key));
	}

	bool InputManagementSystem::IsKeyDown(DJoy a_joykey_key) const
	{
		return IsKeyDown(to_underlying(a_joykey_key));
	}

	bool InputManagementSystem::IsKeyReleased(DJoy a_joykey_key) const
	{
		return IsKeyReleased(to_underlying(a_joykey_key));
	}

	void InputManagementSystem::ProcessInputEvents()
	{
		ClearInputDataBuffers();

		if (!_has_input_events_buffered || !_is_input_enabled)
		{
			return;
		}

		for (const KeyEvent& key_event : _key_event_buffer)
		{
			InputData& data = _input_data_storage[key_event._user_id];
			switch (key_event._event_type)
			{
			case (KeyEventType::KEYBOARD):
			case (KeyEventType::MOUSE):
			{
				if (key_event._action == to_underlying(DKeyAction::PRESSED))
				{
					DKey defined_key			= static_cast<DKey>(key_event._key);
					DKeyAction defined_action	= static_cast<DKeyAction>(key_event._action);

					data._buffered_keys.emplace(to_underlying(defined_key), defined_action);
					data._keys[key_event._key] = true;
				}
				else if (key_event._action == to_underlying(DKeyAction::RELEASED))
				{
					DKey defined_key			= static_cast<DKey>(key_event._key);
					DKeyAction defined_action	= static_cast<DKeyAction>(key_event._action);

					data._buffered_keys.emplace(to_underlying(defined_key), defined_action);
					data._keys[key_event._key] = false;
				}
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
			InputData& data = _input_data_storage[dir_event._user_id];
			switch (dir_event._event_type)
			{
			case (DirectionalEventType::CURSOR):
			{
				data._cursor_position_old.x = data._cursor_position.x;
				data._cursor_position_old.y = data._cursor_position.y;

				data._cursor_delta.x		= dir_event._cursor_x_position - data._cursor_position.x;
				data._cursor_delta.y		= dir_event._cursor_y_position - data._cursor_position.y;

				data._cursor_position.x		= dir_event._cursor_x_position;
				data._cursor_position.y		= dir_event._cursor_y_position;
				break;
			}
			case (DirectionalEventType::SCROLL):
			{
				data._scroll_delta.x		= dir_event._scroll_x_offset;
				data._scroll_delta.y		= dir_event._scroll_y_offset;

				data._scroll_offset_old.x	= data._scroll_offset.x;
				data._scroll_offset_old.y	= data._scroll_offset.y;

				data._scroll_offset.x		+= dir_event._scroll_x_offset;
				data._scroll_offset.y		+= dir_event._scroll_y_offset;
				break;
			}
			case (DirectionalEventType::DEFAULT):
			default:
				DFW_WARNLOG("InputManagement has received an event type it cannot process.");
				break;
			}
		}

		_dir_event_buffer.clear();

		_has_input_events_buffered = false;
	}

	void InputManagementSystem::SendKeyEvent(WindowID a_id, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
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

	void InputManagementSystem::SendMouseEvent(WindowID a_id, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
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

	void InputManagementSystem::SendCharEvent(WindowID a_id, uint16 a_char)
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

	void InputManagementSystem::SendDirectionalEvent(WindowID a_id, float32 a_x_offset, float32 a_y_offset)
	{
		if (_is_input_enabled)
		{
			_has_input_events_buffered = true;
			_dir_event_buffer.emplace_back(a_id, DirectionalEventType::CURSOR, a_x_offset, a_y_offset, 0.0f, 0.0f);
		}
	}

	void InputManagementSystem::SendScrollEvent(WindowID a_id, float32 a_scroll_x_offset, float32 a_scroll_y_offset)
	{
		if (_is_input_enabled)
		{
			_has_input_events_buffered = true;
			_dir_event_buffer.emplace_back(a_id, DirectionalEventType::SCROLL, 0.0f, 0.0f, a_scroll_x_offset, a_scroll_y_offset);
		}
	}

	void InputManagementSystem::RegisterWindow(WindowInstance* a_window)
	{
		InputData& data = _input_data_storage[a_window->_id];
		a_window->_input_data = &data;
	}

	void InputManagementSystem::UnregisterWindow(WindowInstance* a_window)
	{
		_input_data_storage.erase(a_window->_id);
	}

	bool InputManagementSystem::IsKeyPressed(int32 a_key) const
	{
		const WindowInstance* focussed_window = ApplicationInstance::ProvideWindowManagement()->CurrentFocussedWindow();
		if (focussed_window && focussed_window->_input_data)
		{
			const InputData& data = *focussed_window->_input_data;

			auto it_key = data._buffered_keys.find(a_key);
			if (it_key != data._buffered_keys.end())
			{
				if ((*it_key).second == DKeyAction::PRESSED)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool InputManagementSystem::IsKeyDown(int32 a_key) const
	{
		const WindowInstance* focussed_window = ApplicationInstance::ProvideWindowManagement()->CurrentFocussedWindow();
		if (focussed_window && focussed_window->_input_data)
		{
			const InputData& data = *focussed_window->_input_data;
			return data._keys[a_key];
		}

		return false;
	}

	bool InputManagementSystem::IsKeyReleased(int32 a_key) const
	{
		const WindowInstance* focussed_window = ApplicationInstance::ProvideWindowManagement()->CurrentFocussedWindow();
		if (focussed_window && focussed_window->_input_data)
		{
			const InputData& data = *focussed_window->_input_data;

			auto it_key = data._buffered_keys.find(a_key);
			if (it_key != data._buffered_keys.end())
			{
				if ((*it_key).second == DKeyAction::RELEASED)
				{
					return true;
				}
			}
		}

		return false;
	}

	void InputManagementSystem::ClearInputDataBuffers()
	{
		for (auto& [id, input_data] : _input_data_storage)
		{
			input_data._buffered_keys.clear();
			input_data._buffered_characters.clear();
		}
	}

	InputManagementSystem::KeyEvent::KeyEvent(WindowID a_id, KeyEventType a_event_type, int32 a_key, uint16 a_char, int32 a_scancode, int32 a_action, int32 a_modifier)
		: _user_id(a_id)
		, _event_type(a_event_type)
		, _key(a_key)
		, _char(a_char)
		, _scancode(a_scancode)
		, _action(a_action)
		, _modifier(a_modifier)
	{
	}

	InputManagementSystem::KeyEvent::KeyEvent(WindowID a_id, KeyEventType a_event_type, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
		: _user_id(a_id)
		, _event_type(a_event_type)
		, _key(a_key)
		, _scancode(a_scancode)
		, _action(a_action)
		, _modifier(a_modifier)
	{
	}

	InputManagementSystem::KeyEvent::KeyEvent(WindowID a_id, KeyEventType a_event_type,  uint16 a_char)
		: _user_id(a_id)
		, _event_type(a_event_type)
		, _key(to_underlying(DKey::UNDEFINED))
		, _char(a_char)
	{
	}

	InputManagementSystem::DirectionalEvent::DirectionalEvent(WindowID a_id, DirectionalEventType a_event_type, 
		float32 a_cursor_x_pos, float32 a_cursor_y_pos, float32 a_scroll_x_offset, float32 a_scroll_y_offset)
		: _user_id(a_id)
		, _event_type(a_event_type)
		, _cursor_x_position(a_cursor_x_pos)
		, _cursor_y_position(a_cursor_y_pos)
		, _scroll_x_offset(a_scroll_x_offset)
		, _scroll_y_offset(a_scroll_y_offset)
	{
	}

} // End of namespace ~ DCore
