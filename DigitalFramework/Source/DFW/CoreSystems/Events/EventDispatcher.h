#pragma once

#include <entt/signal/dispatcher.hpp>
#include <entt/signal/sigh.hpp>

namespace DFW
{
    // TODO If events take too long to process in a single frame, it might be useful
    // to implement a guard on the maximum number of events that may be queued at a given time.
    // User implementation of handling events should be fast and not halt the frameloop, but might 
    // still be useful.
    // Either dismiss them or queue in separated buffer or even different dispatcher.
    // This needs further research/experimentation if this becomes a problem.
    // Should be able to inherit from entt::dispatcher itself and create a GatedDispatcher.
    class EventDispatcher
    {
    public:
        EventDispatcher() = default;
        ~EventDispatcher() = default;

        template <typename EventType>
        void InstantBroadcast(EventType&& a_event);

        template <typename EventType, typename... TArgs>
        void Broadcast(TArgs&&... a_args);

        template <typename EventType>
        void Broadcast(EventType&& a_event);

        template <typename EventType, auto CallbackType, typename RegistrarType>
        inline void RegisterCallback(RegistrarType&& a_value_or_instance);

        template <typename EventType, auto CallbackType>
        void RegisterCallback();

        template <typename EventType, auto CallbackType, typename RegistrarType>
        inline void UnregisterCallback(RegistrarType&& a_value_or_instance);

        template <typename EventType, auto CallbackType>
        void UnregisterCallback();

        template <typename... EventType>
        bool AreListenersOfTypeConnected();

        template <typename... EventType>
        void ProcessPendingEvents();

        template <typename... EventType>
        void ClearPendingEvents();
    
    private:
        entt::dispatcher _dispatcher;

    };

#pragma region Template Function Implementation

    template <typename EventType>
    void EventDispatcher::InstantBroadcast(EventType&& a_event)
    {
        _dispatcher.trigger(std::forward<EventType>(a_event));
    }

    template <typename EventType, typename... TArgs>
    void EventDispatcher::Broadcast(TArgs&&... a_args)
    {
        _dispatcher.enqueue<EventType>(std::forward<TArgs>(a_args)...);
    }

    template <typename EventType>
    void EventDispatcher::Broadcast(EventType&& a_event)
    {
        _dispatcher.enqueue(std::forward<EventType>(a_event));
    }

    template <typename EventType, auto CallbackType, typename RegistrarType>
    void EventDispatcher::RegisterCallback(RegistrarType&& a_value_or_instance)
    {
        _dispatcher.sink<EventType>().connect<CallbackType>(std::forward<RegistrarType&&>(a_value_or_instance));
    }

    template <typename EventType, auto CallbackType>
    void EventDispatcher::RegisterCallback()
    {
        _dispatcher.sink<EventType>().connect<CallbackType>();
    }

    template <typename EventType, auto CallbackType, typename RegistrarType>
    void EventDispatcher::UnregisterCallback(RegistrarType&& a_value_or_instance)
    {
        _dispatcher.sink<EventType>().disconnect<CallbackType>(std::forward<RegistrarType&&>(a_value_or_instance));
    }

    template <typename EventType, auto CallbackType>
    void EventDispatcher::UnregisterCallback()
    {
        _dispatcher.sink<EventType>().disconnect<CallbackType>();
    }

    template <typename... EventType>
    bool EventDispatcher::AreListenersOfTypeConnected()
    {
        return (!_dispatcher.sink<EventType>().empty() && ...);
    }

    template <typename... EventType>
    void EventDispatcher::ProcessPendingEvents()
    {
        if constexpr (sizeof...(EventType) == 0)
            _dispatcher.update();
        else
            (_dispatcher.update<EventType>(), ...);
    }

    template <typename... EventType>
    void EventDispatcher::ClearPendingEvents()
    {
        if constexpr (sizeof...(EventType) == 0)
            _dispatcher.clear();
        else
            (_dispatcher.clear<EventType>(), ...);
    }

#pragma endregion

} // End of namespace ~ DFW.
