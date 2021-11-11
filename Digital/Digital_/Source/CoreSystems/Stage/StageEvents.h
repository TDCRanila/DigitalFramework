#pragma once

#include <CoreSystems/Stage/StageID.h>
#include <CoreSystems/Events/EventImplementation.h>

#include <AutoFactory/AutoFactory.h>

#include <string>
#include <sstream>

namespace DFW
{
	class StageEvent;

	using StageEventCallbackFunc = std::function<void(StageEvent&)>;

	enum class StageEvents
	{
		StageAttached, StageDetached,
        StageEnabled, StageDisabled,
	};

	class StageEvent : public ReflectedEvent, public DFactory::AutoFactory<StageEvent>
	{
	public:
		StageEvent() = default;
		virtual ~StageEvent()	= default;

	protected:
		StageID			_stage_id;
		std::string		_stage_name;
		bool			_is_stage_disabled;
	};

	class StageControlEvent : public StageEvent
	{
	public:
		StageControlEvent() = default;
		virtual ~StageControlEvent() = default;

		std::string GetDebugString() const
		{
			// TODO Some sort of formatted string would be interesting to implement.
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << "StageID: ";
			debug_string << _stage_id;
			debug_string << " - ";
			debug_string << "StageName: ";
			debug_string << _stage_name;
			debug_string << " - ";
			debug_string << "StageStatus: ";
			if (_is_stage_disabled)
				debug_string << "DISABLED";
			else
				debug_string << "ENABLED";

			return debug_string.str();
		}
	};

	class StageAttachedEvent final : public StageEvent::Registrar<StageAttachedEvent, StageControlEvent>
	{
	public:
		DFW_CONSTRUCT_REFLECTED_EVENT(StageAttachedEvent);

		StageAttachedEvent() = default;
		StageAttachedEvent(StageID a_stage_id, std::string a_stage_name, bool a_stage_is_disabled)
		{
			_stage_id			= a_stage_id;
			_stage_name			= a_stage_name;
			_is_stage_disabled	= a_stage_is_disabled;
		}
	};

	class StageRemovedEvent final : public StageEvent::Registrar<StageRemovedEvent, StageControlEvent>
	{
	public:
		DFW_CONSTRUCT_REFLECTED_EVENT(StageRemovedEvent);

		StageRemovedEvent() = default;
		StageRemovedEvent(StageID a_stage_id, std::string a_stage_name, bool a_stage_is_disabled)
		{
			_stage_id			= a_stage_id;
			_stage_name			= a_stage_name;
			_is_stage_disabled	= a_stage_is_disabled;
		}
	};

	class StageEnabledEvent final : public StageEvent::Registrar<StageEnabledEvent, StageControlEvent>
	{
	public:
		DFW_CONSTRUCT_REFLECTED_EVENT(StageEnabledEvent);

		StageEnabledEvent() = default;
		StageEnabledEvent(StageID a_stage_id, std::string a_stage_name, bool a_stage_is_disabled)
		{
			_stage_id			= a_stage_id;
			_stage_name			= a_stage_name;
			_is_stage_disabled	= a_stage_is_disabled;
		}
	};

	class StageDisabledEvent final : public StageEvent::Registrar<StageDisabledEvent, StageControlEvent>
	{
	public:
		DFW_CONSTRUCT_REFLECTED_EVENT(StageDisabledEvent);

		StageDisabledEvent() = default;
		StageDisabledEvent(StageID a_stage_id, std::string a_stage_name, bool a_stage_is_disabled)
		{
			_stage_id			= a_stage_id;
			_stage_name			= a_stage_name;
			_is_stage_disabled	= a_stage_is_disabled;
		}
	};

} // End of namespace ~ DFW.
