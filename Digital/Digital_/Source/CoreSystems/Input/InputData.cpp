#include <CoreSystems/Input/InputData.h>

namespace DFW
{
	namespace DInput
	{
		InputData::InputData()
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
		}

	} // End of namespace ~ DInput.

} // End of namespace DFW.
