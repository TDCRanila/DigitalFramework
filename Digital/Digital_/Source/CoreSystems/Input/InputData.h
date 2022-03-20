#pragma once

#include <CoreSystems/Input/InputKeys.h>

#include <glm/glm.hpp>

#include <array>
#include <vector>
#include <unordered_map>

namespace DFW
{
	namespace DInput
	{
		struct InputData
		{
			InputData();

			void ClearBuffers();

			glm::vec2 cursor_position;
			glm::vec2 cursor_position_old;
			glm::vec2 cursor_delta;

			glm::vec2 scroll_offset;
			glm::vec2 scroll_offset_old;
			glm::vec2 scroll_delta;

			std::array<DKeyAction, DFW_NUM_KEYINPUT_ENTRIES>	keys { DKeyAction::UNDEFINED };
			std::unordered_map<int16, DKeyAction>				buffered_keys;
			std::vector<uint16>									buffered_characters;

			bool has_buffered_data;

			bool has_mouse_moved_this_frame;
			bool has_received_mouse_input_this_frame;
			bool has_received_scroll_input_this_frame;
			bool has_received_key_input_this_frame;
		};

	} // End of namespace ~ DInput.

} // End of namespace ~ DFW.
