#include <DFW/CoreSystems/Events/EventImplementation.h>

namespace DFW
{
	BaseEvent::BaseEvent() 
		: _is_event_dead(false)
	{
	}

	void BaseEvent::DeclareDead()
	{
		_is_event_dead = true;
	}

	bool BaseEvent::IsDead() const
	{
		return _is_event_dead;
	}
	
	std::string BaseEvent::GetName() const
	{
		return "UNSPECIFIED EVENT NAME";
	}

	std::string BaseEvent::GetDebugString() const
	{
		return std::string();
	}

	int16 BaseEvent::GetEventTypeID() const
	{
		return _event_type_id;
	}
	
} // End of namespace ~ DFW.
