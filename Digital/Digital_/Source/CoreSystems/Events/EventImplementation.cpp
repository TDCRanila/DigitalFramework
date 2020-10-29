#include <CoreSystems/Events/EventImplementation.h>

namespace DCore
{

	BaseEvent::BaseEvent() :
		_is_event_dead(false)
	{
	}

	BaseEvent::~BaseEvent()
	{
	}

	void BaseEvent::SetDead()
	{
		_is_event_dead = true;
	}

	bool BaseEvent::IsDead() const
	{
		return _is_event_dead;
	}
	
	std::string BaseEvent::GetDebugString() const
	{
		return GetName();
	}

	EventDispatcher::EventDispatcher(BaseEvent& a_event)
		: _owned_event(a_event)
	{
	}
	
	EventDispatcher::~EventDispatcher()
	{
	}

} // End of namespace ~ DCore.
