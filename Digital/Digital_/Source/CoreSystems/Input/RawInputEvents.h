#pragma once

#include <CoreSystems/Window/WindowID.h>
#include <CoreSystems/Window/WindowData.h>

namespace DFW
{
	namespace DInput
	{
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

			KeyEventType		event_type;
			DWindow::WindowID	user_id;

			int32	key;
			int32	scancode;
			int32	action;
			int32	modifier;
			uint32	character;
		};

		struct DirectionalEvent
		{
			DirectionalEvent(DWindow::WindowID a_id, DirectionalEventType a_event_type, float32 a_cursor_x_pos, float32 a_cursor_y_pos, float32 a_scroll_x_offset, float32 a_scroll_y_offset);

			DirectionalEventType	event_type;
			DWindow::WindowID		user_id;

			float32 cursor_x_position;
			float32 cursor_y_position;
			float32 scroll_x_offset;
			float32 scroll_y_offset;
		};
	} // End of namespace ~ DInput.

} // End of namespace ~ DFW. 
