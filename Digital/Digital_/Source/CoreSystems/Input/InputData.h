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

			std::array<DKeyAction, 1024>			keys{ DKeyAction::UNDEFINED };
			std::unordered_map<int32, DKeyAction>	buffered_keys;
			std::vector<uint32>						buffered_characters;

			bool has_buffered_data;
		};

	} // End of namespace ~ DInput.

} // End of namespace ~ DFW.
