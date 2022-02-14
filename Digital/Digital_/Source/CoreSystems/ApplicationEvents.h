#pragma once

#include <CoreSystems/Events/EventImplementation.h>

#include <CoreSystems/Window/WindowID.h>

#include <sstream>
#include <functional>

namespace DFW
{
	enum class ApplicationEvents
	{
		WindowCreatedEvent, WindowDestroyedEvent, WindowFocusEvent, WindowMinimizedEvent, WindowMoveEvent, WindowResizeEvent, WindowFramebufferResizeEvent,
		ApplicationCloseEvent, ApplicationResetEvent, ApplicationSettingsSavedEvent,
		GameStartEvent, GamePauseEvent, GameUnpauseEvent, GameResetEvent,
		InputReceivedEvent, InputClipboardEvent, InputItemDropEvent,
		StageAttachedEvent, StageRemovedEvent, StageEnabledEvent, StageDisabledEvent,
		RendererInitializedEvent, RendererTerminatedEvent, RendererAPIChanged
	};

	class ApplicationEvent : public Event
	{
	public:
		ApplicationEvent() = default;
		~ApplicationEvent() = default;

	};

	using ApplicationEventCallbackFunc = std::function<void(ApplicationEvent const&)>;

#pragma region WindowEvents

	/// <summary>
	/// Window Events
	/// </summary>
	
	class WindowCreatedEvent : public ApplicationEvent
	{
	public:
		WindowCreatedEvent(DWindow::WindowID a_window_id)
			: window_id(a_window_id)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, WindowCreatedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << window_id;

			return debug_string.str();
		}

		DWindow::WindowID	window_id;
	};

	class WindowDestroyedEvent : public ApplicationEvent
	{
	public:
		WindowDestroyedEvent(DWindow::WindowID a_window_id)
			: window_id(a_window_id)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, WindowDestroyedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << window_id;

			return debug_string.str();
		}

		DWindow::WindowID	window_id;
	};

	class WindowFocusEvent : public ApplicationEvent
	{
	public:
		WindowFocusEvent(DWindow::WindowID a_window_id, bool a_is_focussed)
			: window_id(a_window_id)
			, is_focussed(a_is_focussed)
		{}			

		DFW_CONSTRUCT_EVENT(ApplicationEvents, WindowFocusEvent);

		std::string GetDebugString() const override 
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << window_id;
			debug_string << " - ";
			debug_string << (is_focussed ? "Focussed" : "Unfocussed");

			return debug_string.str();
		}

		DWindow::WindowID	window_id;
		bool				is_focussed;
	};

	class WindowMinimizedEvent : public ApplicationEvent
	{
	public:
		WindowMinimizedEvent(DWindow::WindowID a_window_id, bool a_is_minimized)
			: window_id(a_window_id)
			, is_minimized(a_is_minimized)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, WindowMinimizedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << window_id;
			debug_string << " - ";
			debug_string << (is_minimized ? "Minimized" : "Opened");

			return debug_string.str();
		}

		DWindow::WindowID	window_id;
		bool				is_minimized;
	};

	class WindowMoveEvent : public ApplicationEvent
	{
	public:
		WindowMoveEvent(DWindow::WindowID a_window_id, int32 a_old_x_pos, int32 a_old_y_pos, int32 a_new_x_pos, int32 a_new_y_pos)
			: window_id(a_window_id)
			, old_x_pos(a_old_x_pos)
			, old_y_pos(a_old_y_pos)
			, new_x_pos(a_new_x_pos)
			, new_y_pos(a_new_y_pos)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, WindowMoveEvent);

		std::string GetDebugString() const override
		{
			// TODO Some sort of formatted string would be interesting to implement.

			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << window_id;
			debug_string << " - ";
			debug_string << " OldPos: ";
			debug_string << old_x_pos;
			debug_string << ",";
			debug_string << old_y_pos;
			debug_string << " - ";
			debug_string << " NewPos: ";
			debug_string << new_x_pos;
			debug_string << ",";
			debug_string << new_y_pos;

			return debug_string.str();
		}

		DWindow::WindowID	window_id;
		int32				old_x_pos;
		int32				old_y_pos;
		int32				new_x_pos;
		int32				new_y_pos;
	};

	class WindowResizeEvent : public ApplicationEvent
	{
	public:
		WindowResizeEvent(DWindow::WindowID a_window_id, int32 a_old_width, int32 a_old_height, int32 a_new_width, int32 a_new_height)
			: window_id(a_window_id)
			, old_width(a_old_width)
			, old_height(a_old_height)
			, new_width(a_new_width)
			, new_height(a_new_height)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, WindowResizeEvent);

		std::string GetDebugString() const override
		{
			// TODO Some sort of formatted string would be interesting to implement.

			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << window_id;
			debug_string << " - ";
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

		DWindow::WindowID	window_id;
		int32				old_width;
		int32				old_height;
		int32				new_width;
		int32				new_height;
	};

	class WindowFramebufferResizeEvent : public WindowResizeEvent
	{
	public:
		WindowFramebufferResizeEvent(DWindow::WindowID a_window_id, int32 a_old_width, int32 a_old_height, int32 a_new_width, int32 a_new_height)
			: WindowResizeEvent(a_window_id, a_old_width, a_old_height, a_new_width, a_new_height)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, WindowFramebufferResizeEvent);

	};

#pragma endregion

#pragma region ApplicationEvents

	/// <summary>
	/// General Application Events
	/// </summary>
	class ApplicationCloseEvent : public ApplicationEvent
	{
	public:
		ApplicationCloseEvent() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, ApplicationCloseEvent);
	};

	class ApplicationResetEvent : public ApplicationEvent
	{
	public:
		ApplicationResetEvent() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, ApplicationResetEvent);
	};

	class ApplicationSettingsSavedEvent : public ApplicationEvent
	{
	public:
		ApplicationSettingsSavedEvent() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, ApplicationSettingsSavedEvent);
	};

#pragma endregion

#pragma region GameEvents

	/// <summary>
	/// Game Events
	/// </summary>
	class GameStartEvent : public ApplicationEvent
	{
	public:
		GameStartEvent() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, GameStartEvent);
	};

	class GamePauseEvent : public ApplicationEvent
	{
	public:
		GamePauseEvent(bool a_paused)
			: _paused(a_paused)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, GamePauseEvent);

		bool _paused;
	};

	class GameResetEvent : public ApplicationEvent
	{
	public:
		GameResetEvent() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, GameResetEvent);
	};

#pragma endregion

#pragma region InputEvents

	/// <summary>
	/// Input Events
	/// </summary>
	class InputReceivedEvent : public ApplicationEvent
	{
	public:
		InputReceivedEvent() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, InputReceivedEvent);
	};

	class InputClipboardEvent : public ApplicationEvent
	{
	public:
		InputClipboardEvent(const char* a_clipboard_input) 
			: _clipboard_input(a_clipboard_input)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, InputClipboardEvent);

		std::string GetDebugString() const override
		{
			// TODO Some sort of formatted string would be interesting to implement.
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - Fetched Clipboard: ";
			debug_string << _clipboard_input;

			return debug_string.str();
		}

		const char* _clipboard_input;
	};

	class InputItemDropEvent : public ApplicationEvent
	{
	public:
		InputItemDropEvent(int32 a_item_count, const char** a_item_paths) 
			: _item_count(a_item_count)
			, _item_paths(a_item_paths)
		{}

		DFW_CONSTRUCT_EVENT(ApplicationEvents, InputItemDropEvent);

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

		int32			_item_count;
		const char**	_item_paths;
	};

#pragma endregion

#pragma region RenderEvents

	class RendererInitializedEvent : public ApplicationEvent
	{
	public:
		RendererInitializedEvent() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, RendererInitializedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();

			return debug_string.str();
		}

	};

	class RendererTerminatedEvent : public ApplicationEvent
	{
	public:
		RendererTerminatedEvent() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, RendererTerminatedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();

			return debug_string.str();
		}

	};

	class RendererAPIChanged : public ApplicationEvent
	{
	public:
		RendererAPIChanged() = default;

		DFW_CONSTRUCT_EVENT(ApplicationEvents, RendererAPIChanged);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();

			return debug_string.str();
		}

	};

#pragma endregion 

} // End of namespace ~ DFW.
