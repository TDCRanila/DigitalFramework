#pragma once

#include <CoreSystems/Logging/Logger.h>

#include <Utility/TemplateUtility.h>

#include <functional>

namespace DCore
{
	class BaseEvent
	{
	public:
		BaseEvent();
		virtual ~BaseEvent() = default;
		
		void DeclareDead();
		bool IsDead() const;

		virtual std::string GetName() const;
		virtual std::string GetDebugString() const;

		operator std::string() const { return GetName(); }

		// TODO Add additional compare operators for specialization. See EventDispatcher.
		bool operator==(const BaseEvent& a_other) { return this->_event_type_id == a_other._event_type_id; }

		virtual int16 GetEventTypeID() const;

	protected:
		friend class EventLibrary;

		template <typename OwnedEventType>
		requires EventDispatcherRequirements<OwnedEventType>
		friend class EventDispatcher;

		inline static int16 _event_type_id = 0; /*8bits;event category - 8bits;event type*/

		struct BaseTag {};
		struct ReflectedTag {};
		struct EventTag {};

		using TypeTag = BaseTag;

	private:
		bool _is_event_dead;

	};

	class Event : public BaseEvent
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		using TypeTag = EventTag;
	};

	typedef std::function<void(BaseEvent&)> EventCallbackFunc;

	class ReflectedEvent : public BaseEvent
	{
	public:
		ReflectedEvent() = default;
		virtual ~ReflectedEvent() = default;

		using TypeTag = ReflectedTag;
	};

#define DFW_CONSTRUCT_EVENT(a_event_enum, a_type)	static constexpr a_event_enum GetStaticType()	{ return a_event_enum::a_type; }				\
													a_event_enum GetType() const					{ return GetStaticType(); }						\
													virtual int16 GetEventTypeID() const override	{ return static_cast<int16>(to_underlying(a_event_enum::a_type)); }	\
													virtual std::string GetName() const override	{ return #a_type; }		\
													using TypeTag = EventTag;
	 

#define DFW_CONSTRUCT_REFLECTED_EVENT(a_type)	static constexpr int16 GetStaticType()			{ return a_type::_event_type_id; }	\
												virtual int16 GetEventTypeID() const override	{ return a_type::_event_type_id; }	\
												virtual std::string GetName() const override	{ return #a_type; } \
												using TypeTag = ReflectedTag;

	enum class EventAction
	{
		None, DeclareDeadOnSucces, DeclareDead
	};
	
	template <typename OwnedEventType>
	concept EventDispatcherRequirements = IsDerivedFrom<OwnedEventType, BaseEvent> and not AreSameTypes<OwnedEventType, Event>;

	template <typename OwnedEventType>
	requires EventDispatcherRequirements<OwnedEventType>
	class EventDispatcher
	{
	public:
		EventDispatcher(OwnedEventType& a_event)
			: _owned_event(a_event)
		{
			// TODO DEBUG TAG/IfElse.
			DFW_LOG("Starting EventDispatcher with type: {}", typeid(OwnedEventType).name());
		}

		~EventDispatcher() = default;

		template <typename EventType, typename Func>
		bool Dispatch(const Func& a_func, EventAction a_event_action);

	private:
		OwnedEventType& _owned_event;

	};

} // End of namespace ~ Dcore.

#pragma region Template Implementation

// https://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class
// TODO: Add new Dispatch function that only works for T types that have type; (FactoryMap)T::GetFactories. 
// This is for the reflected events. // otherwise default to the normal one.

namespace DCore
{
	template <typename OwnedEventType>
	requires EventDispatcherRequirements<OwnedEventType>
	template <typename EventType, typename Func>
	bool EventDispatcher<OwnedEventType>::Dispatch(const Func& a_func, EventAction a_event_action)
	{
		if constexpr (not IsDerivedFrom<EventType, BaseEvent>)
		{
			static_assert(IsAlwaysFalse<EventType>, __FUNCTION__ " - EventType's type is not derived from DCore::BaseEvent.");
			return false;
		}

		if (_owned_event.IsDead())
			return false;

		bool comparison_result = false;
		
		if constexpr (
			// BaseEvent/ReflectedEvent === ReflectedEvent
			(AreSameTypes<OwnedEventType::TypeTag, BaseEvent::ReflectedTag> or AreSameTypes<OwnedEventType::TypeTag, BaseEvent::BaseTag>)
			and AreSameTypes<EventType::TypeTag, BaseEvent::ReflectedTag>
			and IsDerivedFrom<EventType, OwnedEventType>)
		{
			comparison_result = _owned_event.GetEventTypeID() == EventType::GetStaticType();
		}
		else if constexpr (
			// Event(type A) === Event(type A)
			AreSameTypes<OwnedEventType::TypeTag, BaseEvent::EventTag>
			and AreSameTypes<EventType::TypeTag, BaseEvent::EventTag>)
		{
			// Event(type A) === Event(type B) : Do not allow comparison between types using different enum classes.
			if constexpr (not IsDerivedFrom<EventType, OwnedEventType>)
			{
				static_assert(IsAlwaysFalse<EventType>, __FUNCTION__ " - EventType's type must be derived from the same (non-reflected) Event Type.");
				return false;
			}
			else
			{
				comparison_result = _owned_event.GetEventTypeID() == static_cast<int16>(to_underlying(EventType::GetStaticType()));
			}
		}
		else
		{
			// No comparison possible.
			comparison_result = false;
		}

		// TODO DEBUG TAG/IfElse.
		DFW_LOG("Attemping to dispatch with configuration: Result; {}, ReceivingEvent; {}, CompareEvent; {} - CallFunc; {}",
			comparison_result, typeid(OwnedEventType).name(), typeid(EventType).name(), typeid(Func).name());

		if (comparison_result)
			a_func();

		if (a_event_action == EventAction::DeclareDead)
			_owned_event.DeclareDead();

		if (a_event_action == EventAction::DeclareDeadOnSucces && comparison_result)
			_owned_event.DeclareDead();

		return comparison_result;
	}

} // End of namespace ~ DCore.

#pragma endregion
