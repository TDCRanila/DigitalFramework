#include <CoreSystems/Input/InputData.h>

namespace DFW
{
	namespace DInput
	{
		InputData::InputData()
			: cursor_position(0.0f)
			, cursor_position_old(0.0f)
			, cursor_delta(0.0f)

			, scroll_offset(0.0f)
			, scroll_offset_old(0.0f)
			, scroll_delta(0.0f)
			
			, has_buffered_data(false)
			
			, has_mouse_moved_this_frame(false)
			, has_received_mouse_input_this_frame(false)
			, has_received_scroll_input_this_frame(false)
			, has_received_key_input_this_frame(false)
		{
			buffered_keys.reserve(16);
			buffered_characters.reserve(16);
		}

		void InputData::ClearBuffers()
		{
			if (!has_buffered_data)
				return;

			has_buffered_data = false;

			buffered_keys.clear();
			buffered_characters.clear();

			cursor_delta = glm::vec2(0.0f);
			scroll_delta = glm::vec2(0.0f);

			has_mouse_moved_this_frame				= false;
			has_received_mouse_input_this_frame		= false;
			has_received_scroll_input_this_frame	= false;
			has_received_key_input_this_frame		= false;
		}

	} // End of namespace ~ DInput.

} // End of namespace DFW.
