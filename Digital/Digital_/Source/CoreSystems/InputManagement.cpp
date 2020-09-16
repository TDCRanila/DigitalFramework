#include <CoreSystems/InputManagement.h>

#include <Defines/InputDefines.h>

namespace DCore
{
	InputUserData::InputUserData(InputUserID a_id, InputManagementSystem* a_input_system_ptr)
		:	_user_id(a_id),
			_input_system_ptr(a_input_system_ptr)
	{
	}

	InputData::InputData() 
		:	_cursor_position(0.0f),
			_cursor_position_old(0.0f),
			_cursor_delta(0.0f),
			_scroll_offset(0.0f)
	{
		_buffered_keys.reserve(16);
	}

	InputData::BufferedKey::BufferedKey(int32 a_key, int32 a_action)
		:	_key(a_key),
			_action(a_action)
	{
	}

	InputManagementSystem::InputManagementSystem()
		:	_has_input_events_buffered(false),
			_is_input_enabled(true)
	{
		_keyboard_event_buffer.reserve(16);
		_mouse_event_buffer.reserve(16);
		_cursor_event_buffer.reserve(16);
		_scroll_event_buffer.reserve(16);
	}

	InputManagementSystem::~InputManagementSystem()
	{

	}

	InputUserData* InputManagementSystem::ProvideInputUserData(const WindowInstance* a_window_instance)
	{
		// TODO Some kind of proper id/identifer for which window uses what input data
		InputUserID id = reinterpret_cast<int64>(a_window_instance);
		_input_data_storage.try_emplace(0/*id*/);


		InputUserData* input_user_data = &_input_user_data_storage.emplace_back(0, this);
		return input_user_data;
	}

	void InputManagementSystem::ProcessInputEvents()
	{
		for (auto& input_data : _input_data_storage)
		{
			input_data.second._buffered_keys.clear();
			input_data.second._frame_buffered_characters.clear();
		}

		if (!_has_input_events_buffered || !_is_input_enabled)
		{
			return;
		}

		for (const KeyboardEvent& event : _keyboard_event_buffer)
		{
			auto it = _input_data_storage.find(event._user_id);
			InputData& input_data = it->second;

			if ((event._key >= 0) && (event._key < 1024))
			{
			    if (event._action == GLFW_PRESS)
				{ 
					input_data._buffered_keys.emplace_back(event._key, event._action);
					input_data._keys[event._key] = true;
				}
			    else if (event._action == GLFW_RELEASE)
				{ 
					input_data._buffered_keys.emplace_back(event._key, event._action);
					input_data._keys[event._key] = false;
				}
				INFOLOG("Key Pressed: " << event._key);
			}
		}

		for (const CharacterEvent& event : _character_event_buffer)
		{
			auto it = _input_data_storage.find(event._user_id);
			InputData& input_data = it->second;

			if (event._char >= 0)
			{
				input_data._frame_buffered_characters.emplace_back(event._char);
				INFOLOG("Char Inserted: " << event._char);
			}
		}

		for (const MouseEvent& event : _mouse_event_buffer)
		{
			auto it = _input_data_storage.find(event._user_id);
			InputData& input_data = it->second;

			// TODO Create Key Event to trigger all registered callbacks

			if ((event._key >= 0) && (event._key < 8)) 
			{
				if (event._action == KEY_PRESS) 
				{ 
					input_data._buffered_keys.emplace_back(event._key, event._action);
					input_data._keys[event._key] = true; 
				}
				else if (event._action == KEY_RELEASE) 
				{ 
					input_data._buffered_keys.emplace_back(event._key, event._action);
					input_data._keys[event._key] = false; 
				}
				INFOLOG("Key Pressed: " << event._key);
			}

		}

		for (const CursorEvent& event : _cursor_event_buffer)
		{
			auto it = _input_data_storage.find(event._user_id);
			InputData& input_data = it->second;

			input_data._cursor_position_old.x = static_cast<float32>(input_data._cursor_position.x);
			input_data._cursor_position_old.y = static_cast<float32>(input_data._cursor_position.y);

			input_data._cursor_delta.x = static_cast<float32>(static_cast<float32>(event._x_pos) - input_data._cursor_position.x);
			input_data._cursor_delta.y = static_cast<float32>(static_cast<float32>(event._y_pos) - input_data._cursor_position.y);

			input_data._cursor_position.x = static_cast<float32>(event._x_pos);
			input_data._cursor_position.y = static_cast<float32>(event._y_pos);

			INFOLOG("X:" << event._x_pos << " -  Y:" << event._y_pos );
			INFOLOG("XOffset:" << input_data._cursor_delta.x << " -  YOffset:" << input_data._cursor_delta.y);
		}

		for (const ScrollEvent& event : _scroll_event_buffer)
		{
			auto it = _input_data_storage.find(event._user_id);
			InputData& input_data = it->second;

			input_data._scroll_offset.y += event._y_offset;

			INFOLOG("ScrollXOffset:" << input_data._scroll_offset.x << " -  ScrollYOffset:" << input_data._scroll_offset.y);
		}

		_keyboard_event_buffer.clear();
		_character_event_buffer.clear();
		_mouse_event_buffer.clear();
		_cursor_event_buffer.clear();
		_scroll_event_buffer.clear();

		_has_input_events_buffered = false;
	}

	void InputManagementSystem::SendKeyboardEvent(InputUserData* a_input_user_data, int a_key, int a_scancode, int a_action, int a_mods)
	{
		if (_is_input_enabled)
		{
			_has_input_events_buffered = true;
			_keyboard_event_buffer.emplace_back(a_input_user_data->_user_id, a_key, a_scancode, a_action, a_mods);
		}
	}

	void InputManagementSystem::SendCharEvent(InputUserData* a_input_user_data, unsigned int a_char)
	{
		if (_is_input_enabled)
		{
			_has_input_events_buffered = true;
			_character_event_buffer.emplace_back(a_input_user_data->_user_id, a_char);
		}
	}

	void InputManagementSystem::SendMouseEvent(InputUserData* a_input_user_data, int a_key, int a_action, int a_mods)
	{
		if (_is_input_enabled)
		{
			_has_input_events_buffered = true;
			_mouse_event_buffer.emplace_back(a_input_user_data->_user_id, a_key, a_action, a_mods);
		}
	}

	void InputManagementSystem::SendCursorEvent(InputUserData* a_input_user_data, double a_x, double a_y)
	{
		if (_is_input_enabled)
		{
			_has_input_events_buffered = true;
			_cursor_event_buffer.emplace_back(a_input_user_data->_user_id, a_x, a_y);
		}
	}

	void InputManagementSystem::SendScrollEvent(InputUserData* a_input_user_data, double a_x_offset, double a_y_offset)
	{
		if (_is_input_enabled)
		{
			_has_input_events_buffered = true;
			_scroll_event_buffer.emplace_back(a_input_user_data->_user_id, a_x_offset, a_y_offset);
		}
	}

	InputManagementSystem::KeyboardEvent::KeyboardEvent(InputUserID a_id, int key, int scancode, int action, int mods)
		:	_user_id(a_id),
			_key(key),
			_scancode(scancode),
			_action(action),
			_modifier(mods)
	{
	}

	InputManagementSystem::CharacterEvent::CharacterEvent(InputUserID a_id, unsigned int a_char)
		:	_user_id(a_id),
			_char(a_char)
	{
	}

	InputManagementSystem::MouseEvent::MouseEvent(InputUserID a_id, int a_key, int action, int mods)
		:	_user_id(a_id),
			_key(a_key),
			_action(action),
			_modifier(mods)
	{
	}

	InputManagementSystem::CursorEvent::CursorEvent(InputUserID a_id, double a_x, double a_y) 
		:	_user_id(a_id),
			_x_pos(a_x),
			_y_pos(a_y)
	{
	}

	InputManagementSystem::ScrollEvent::ScrollEvent(InputUserID a_id, double a_x_offset, double a_y_offset)
		:	_user_id(a_id),
			_x_offset(a_x_offset),
			_y_offset(a_y_offset)
	{
	}

} // End of namespace ~ DCore
