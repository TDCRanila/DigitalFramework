#pragma once

#include <glm/glm.hpp>

#include <array>
#include <functional>

namespace DCore
{
	struct WindowInstance;

	struct InputData;
	class InputManagementSystem;

	typedef int64 InputUserID;

	struct InputUserData
	{
		InputUserData(InputUserID a_id, InputManagementSystem* a_input_system_ptr);

		InputUserID _user_id;
		InputManagementSystem* _input_system_ptr;
	};

	struct InputData
	{
		InputData();

		struct BufferedKey
		{
			BufferedKey(int32 a_key, int32 a_action);

			int32 _key;
			int32 _action;
		};

		std::array<bool, 1024> _keys{false};
		std::vector<BufferedKey> _buffered_keys;
		std::vector<uint32> _frame_buffered_characters;

		glm::vec2 _cursor_position;
		glm::vec2 _cursor_position_old;
		glm::vec2 _cursor_delta;
		glm::vec2 _scroll_offset;

	};

	class InputManagementSystem
	{
	public:

		InputManagementSystem();
		~InputManagementSystem();

		void ProcessInputEvents();

		InputUserData* ProvideInputUserData(const WindowInstance* a_window_instance);

		void RegisterKeyEvent(std::string a_key_event , int64 a_key, std::string a_input_profile = "default");
		
		void RegisterInput(std::string a_key_event, std::function<void> a_function , std::string a_input_profile = "default");

		void DefaultInputProfile();
		void ChangeActiveInputProfile(std::string a_new_input_profile);

		void EnableInput();
		void DisableInput();

		void SendKeyboardEvent(InputUserData* a_input_user_data, int a_key, int a_scancode, int a_action, int a_mods);
		void SendCharEvent(InputUserData* a_input_user_data, unsigned int a_char);
		void SendMouseEvent(InputUserData* a_input_user_data, int a_key, int a_action, int a_mods);
		void SendCursorEvent(InputUserData* a_input_user_data, double a_x, double a_y);
		void SendScrollEvent(InputUserData* a_input_user_data, double a_x_offset, double a_y_offset);

		// TODO std::string should be some kind of ID
		std::unordered_map<InputUserID, InputData> _input_data_storage;
		std::vector<InputUserData> _input_user_data_storage;

	protected:

	private:

		struct KeyboardEvent
		{
			KeyboardEvent(InputUserID a_id, int key, int scancode, int action, int mods);

			InputUserID _user_id;
			int32 _key;
			int32 _scancode;
			int32 _action;
			int32 _modifier;
		};

		struct CharacterEvent
		{
			CharacterEvent(InputUserID a_id, unsigned int a_char);

			InputUserID _user_id;
			uint32 _char;
		};
		
		struct MouseEvent
		{
			MouseEvent(InputUserID a_id, int a_key, int a_action, int a_mods);

			InputUserID _user_id;
			int32 _key;
			int32 _scancode;
			int32 _action;
			int32 _modifier;
		};

		struct CursorEvent
		{
			CursorEvent(InputUserID a_id, double a_x, double a_y);

			InputUserID _user_id;
			float64 _x_pos;
			float64 _y_pos;
		};

		struct ScrollEvent
		{
			ScrollEvent(InputUserID a_id, double a_x, double a_y);

			InputUserID _user_id;
			float64 _x_offset;
			float64 _y_offset;
		};

		std::vector<KeyboardEvent> _keyboard_event_buffer;
		std::vector<CharacterEvent> _character_event_buffer;
		std::vector<MouseEvent> _mouse_event_buffer;
		std::vector<CursorEvent> _cursor_event_buffer;
		std::vector<ScrollEvent> _scroll_event_buffer;
		
		bool _has_input_events_buffered;
		bool _is_input_enabled;

	};

} // End of namespace ~ DCore