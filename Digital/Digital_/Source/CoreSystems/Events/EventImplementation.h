#pragma once

#include <CoreSystems/Logging/Logger.h>

#include <Utility/TemplateUtility.h>

#include <functional>

namespace DFW
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


} // End of namespace ~ DFW.
