#include <DFW/CoreSystems/Input/RawInputEvents.h>

#include <DFW/CoreSystems/Input/InputKeys.h>

namespace DFW
{
    namespace DInput
    {
		KeyEvent::KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, int32 a_key, uint16 a_char, int32 a_scancode, int32 a_action, int32 a_modifier)
			: user_id(a_id)
			, event_type(a_event_type)
			, key(a_key)
			, character(a_char)
			, scancode(a_scancode)
			, action(a_action)
			, modifier(a_modifier)
		{
		}

		KeyEvent::KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, int32 a_key, int32 a_scancode, int32 a_action, int32 a_modifier)
			: user_id(a_id)
			, event_type(a_event_type)
			, key(a_key)
			, scancode(a_scancode)
			, action(a_action)
			, modifier(a_modifier)
		{
		}

		KeyEvent::KeyEvent(DWindow::WindowID a_id, KeyEventType a_event_type, uint16 a_char)
			: user_id(a_id)
			, event_type(a_event_type)
			, key(to_underlying(DKey::UNDEFINED))
			, character(a_char)
		{
		}

		DirectionalEvent::DirectionalEvent(DWindow::WindowID a_id, DirectionalEventType a_event_type,
			float32 a_cursor_x_pos, float32 a_cursor_y_pos, float32 a_scroll_x_offset, float32 a_scroll_y_offset)
			: user_id(a_id)
			, event_type(a_event_type)
			, cursor_x_position(a_cursor_x_pos)
			, cursor_y_position(a_cursor_y_pos)
			, scroll_x_offset(a_scroll_x_offset)
			, scroll_y_offset(a_scroll_y_offset)
		{
		}
    } // End of namespace ~ DInput.

} // End of namespace ~ DFW.
