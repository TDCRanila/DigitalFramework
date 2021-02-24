#pragma once

#include <functional>

namespace DCore
{

	class BaseEvent;

	typedef BaseEvent ApplicationEvent;

	typedef std::function<void(BaseEvent&)> EventCallbackFunc;

	enum class AppEvent
	{
		WindowCloseEvent, WindowFocusEvent, WindowMinimizedEvent, WindowMoveEvent, WindowResizeEvent, WindowFramebufferResizeEvent,
		ApplicationCloseEvent, ApplicationResetEvent, ApplicationSettingsSavedEvent,
		GameStartEvent, GamePauseEvent, GameUnpauseEvent, GameResetEvent,
		InputReceivedEvent, InputClipboardEvent, InputItemDropEvent

	};

	class BaseEvent
	{
	public:
		BaseEvent();
		~BaseEvent();

		void SetDead();
		bool IsDead() const;

		virtual AppEvent GetType() const	= 0;
		virtual std::string GetName() const	= 0;
		virtual std::string GetDebugString() const;

		operator std::string() const
		{
			return GetName();
		}
				
	private:
		bool _is_event_dead;

	};

	class EventDispatcher
	{
	public:
		EventDispatcher(BaseEvent& a_event);
		~EventDispatcher();

		template<typename T, typename Func>
		bool Dispatch(const Func& a_func);

	private:
		BaseEvent& _owned_event;

	};

#define DFW_REGISTER_EVENT(a_type)	static AppEvent GetStaticType()					{ return AppEvent::a_type; }	\
									virtual AppEvent GetType() const override		{ return GetStaticType(); }		\
									virtual std::string GetName() const override	{ return #a_type; }				

} // End of namespace ~ Dcore.

#pragma region Template Implementation

namespace DCore
{
	template<typename T, typename Func>
	bool EventDispatcher::Dispatch(const Func& a_func)
	{
		if (_owned_event.GetEventType() != T::GetEventType())
			return false;

		a_func(_owned_event);
		return true;
	}

} // End of namespace ~ DCore.

#pragma endregion
