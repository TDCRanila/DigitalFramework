#pragma once

#include <CoreSystems/Events/EventImplementation.h>

#include <CoreSystems/Window/WindowData.h>

#include <sstream>

namespace DCore
{
	/* enum class AppEvent
		WindowCloseEvent, WindowFocusEvent, WindowMinimizedEvent, WindowMoveEvent, WindowResizeEvent, WindowFramebufferResizeEvent,
		ApplicationCloseEvent, ApplicationResetEvent, ApplicationSettingsSavedEvent,
		GameStartEvent, GamePauseEvent, GameUnpauseEvent, GameResetEvent,
		InputReceivedEvent, InputClipboardEvent, InputItemDropEvent
	*/

	/// <summary>
	/// Window Events
	/// </summary>
	class WindowCloseEvent : public ApplicationEvent
	{
	public:
		WindowCloseEvent() = default;

		DFW_REGISTER_EVENT(WindowCloseEvent);
	};

	class WindowFocusEvent : public ApplicationEvent
	{
	public:
		WindowFocusEvent(bool a_is_focussed)
			: _is_focussed(a_is_focussed)
		{}			

		DFW_REGISTER_EVENT(WindowFocusEvent);

		std::string GetDebugString() const override 
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << (_is_focussed ? "Focussed" : "Unfocussed");

			return debug_string.str();
		}

		const bool _is_focussed;
	};

	class WindowMinimizedEvent : public ApplicationEvent
	{
	public:
		WindowMinimizedEvent(bool a_is_minimized)
			: _is_minimized(a_is_minimized)
		{}

		DFW_REGISTER_EVENT(WindowMinimizedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << (_is_minimized ? "Minimized" : "Opened");

			return debug_string.str();
		}

		const bool _is_minimized;
	};

	class WindowMoveEvent : public ApplicationEvent
	{
	public:
		WindowMoveEvent(int32 a_old_x_pos, int32 a_old_y_pos, int32 a_new_x_pos, int32 a_new_y_pos)
			: _old_x_pos(a_old_x_pos)
			, _old_y_pos(a_old_y_pos)
			, _new_x_pos(a_new_x_pos)
			, _new_y_pos(a_new_y_pos)
		{}

		DFW_REGISTER_EVENT(WindowMoveEvent);

		std::string GetDebugString() const override
		{
			// TODO Some sort of formatted string would be interesting to implement.

			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " OldPos: ";
			debug_string << _old_x_pos;
			debug_string << ",";
			debug_string << _old_y_pos;
			debug_string << " - ";
			debug_string << " NewPos: ";
			debug_string << _new_x_pos;
			debug_string << ",";
			debug_string << _new_y_pos;

			return debug_string.str();
		}

		const int32 _old_x_pos;
		const int32 _old_y_pos;
		const int32 _new_x_pos;
		const int32 _new_y_pos;
	};

	class WindowResizeEvent : public ApplicationEvent
	{
	public:
		WindowResizeEvent(int32 a_old_width, int32 a_old_height, int32 a_new_width, int32 a_new_height) 
			: old_width(a_old_width)
			, old_height(a_old_height)
			, new_width(a_new_width)
			, new_height(a_new_height)
		{}

		DFW_REGISTER_EVENT(WindowResizeEvent);

		std::string GetDebugString() const override
		{
			// TODO Some sort of formatted string would be interesting to implement.

			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " OldRes: ";
			debug_string << old_width;
			debug_string << ",";
			debug_string << old_height;
			debug_string << " - ";
			debug_string << " NewRes: ";
			debug_string << new_width;
			debug_string << ",";
			debug_string << new_height;

			return debug_string.str();
		}

		const int32 old_width;
		const int32 old_height;
		const int32 new_width;
		const int32 new_height;
	};

	class WindowFramebufferResizeEvent : public WindowResizeEvent
	{
	public:
		WindowFramebufferResizeEvent(int32 a_old_width, int32 a_old_height, int32 a_new_width, int32 a_new_height)
			: WindowResizeEvent(a_old_width, a_old_height, a_new_width, a_new_height)
		{}

		DFW_REGISTER_EVENT(WindowFramebufferResizeEvent);

	};

	/// <summary>
	/// General Application Events
	/// </summary>
	class ApplicationCloseEvent : public ApplicationEvent
	{
	public:
		ApplicationCloseEvent() = default;

		DFW_REGISTER_EVENT(ApplicationCloseEvent);
	};

	class ApplicationResetEvent : public ApplicationEvent
	{
	public:
		ApplicationResetEvent() = default;

		DFW_REGISTER_EVENT(ApplicationResetEvent);
	};

	class ApplicationSettingsSavedEvent : public ApplicationEvent
	{
	public:
		ApplicationSettingsSavedEvent() = default;

		DFW_REGISTER_EVENT(ApplicationSettingsSavedEvent);
	};

	/// <summary>
	/// Game Events
	/// </summary>
	class GameStartEvent : public ApplicationEvent
	{
	public:
		GameStartEvent() = default;

		DFW_REGISTER_EVENT(GameStartEvent);
	};

	class GamePauseEvent : public ApplicationEvent
	{
	public:
		GamePauseEvent(bool a_paused)
			: _paused(a_paused)
		{}

		DFW_REGISTER_EVENT(GamePauseEvent);

		const bool _paused;
	};

	class GameResetEvent : public ApplicationEvent
	{
	public:
		GameResetEvent() = default;

		DFW_REGISTER_EVENT(GameResetEvent);
	};

	/// <summary>
	/// Input Events
	/// </summary>
	class InputReceivedEvent : public ApplicationEvent
	{
	public:
		InputReceivedEvent() = default;

		DFW_REGISTER_EVENT(InputReceivedEvent);
	};

	class InputClipboardEvent : public ApplicationEvent
	{
	public:
		InputClipboardEvent(const char* a_clipboard_input) 
			: _clipboard_input(a_clipboard_input)
		{}

		DFW_REGISTER_EVENT(InputClipboardEvent);

		std::string GetDebugString() const override
		{
			// TODO Some sort of formatted string would be interesting to implement.
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - Fetched Clipboard: ";
			debug_string << _clipboard_input;

			return debug_string.str();
		}

		const char* const _clipboard_input;
	};

	class InputItemDropEvent : public ApplicationEvent
	{
	public:
		InputItemDropEvent(int32 a_item_count, const char** a_item_paths) 
			: _item_count(a_item_count)
			, _item_paths(a_item_paths)
		{}

		DFW_REGISTER_EVENT(InputItemDropEvent);

		std::string GetDebugString() const override
		{
			// TODO Some sort of formatted string would be interesting to implement.
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			
			if (_item_count == 1)
			{
				debug_string << "{";
				debug_string << _item_paths[0];
				debug_string << "} ";
			}
			else
			{
				for (int32 it = 0; it < _item_count; ++it)
				{
					debug_string << "{";
					debug_string << _item_paths[it];
					debug_string << "}, ";
				}
			}

			return debug_string.str();
		}

		const int32			_item_count;
		const char** const	_item_paths;
	};

} // End of namespace ~ DCore
