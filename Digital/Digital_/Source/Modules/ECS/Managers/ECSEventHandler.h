#pragma once

#include <entt/signal/dispatcher.hpp>
#include <entt/signal/sigh.hpp>

namespace DECS
{
    // FW Declare
    class ECSModule;

    // TODO If events take too long to process in a single frame, it might be useful
    // to implement a guard on the maximum number of events that may be queued at a given time.
    // User implementation of handling events should be fast and not halt the frameloop, but might 
    // still be useful.
    // Either dismiss them or queue in separated buffer or even different dispatcher.
    // This needs further research/experimentation if this becomes a problem.
    class ECSEventHandler
    {
    public:
        ECSEventHandler() = default;
        ~ECSEventHandler() = default;

        template <typename EventType, typename... TArgs>
        void InstantBroadcast(TArgs&&... a_args);

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

    protected:
        friend ECSModule;
        
        void Init();
        void Terminate();

        template <typename... EventType>
        void ProcessPendingEvents();

        template <typename... EventType>
        void ClearPendingEvents();

    private:
        entt::dispatcher _dispatcher;

    }; 

#pragma region Template Function Implementation

    template <typename EventType, typename... TArgs>
    void ECSEventHandler::InstantBroadcast(TArgs&&... a_args)
    {
        _dispatcher.trigger<EventType>(std::forward<TArgs&&>(a_args)...);
    }

    template <typename EventType>
    void ECSEventHandler::InstantBroadcast(EventType&& a_event)
    {
        _dispatcher.trigger(std::forward<EventType>(a_event));
    }

    template <typename EventType, typename... TArgs>
    void ECSEventHandler::Broadcast(TArgs&&... a_args)
    {
        _dispatcher.enqueue<EventType>(std::forward<TArgs>(a_args)...);
    }

    template <typename EventType>
    void ECSEventHandler::Broadcast(EventType&& a_event)
    {
        _dispatcher.enqueue(std::forward<EventType>(a_event));
    }

    template <typename EventType, auto CallbackType, typename RegistrarType>
    void ECSEventHandler::RegisterCallback(RegistrarType&& a_value_or_instance)
    {
        _dispatcher.sink<EventType>().connect<CallbackType>(std::forward<RegistrarType&&>(a_value_or_instance));
    }

    template <typename EventType, auto CallbackType>
    void ECSEventHandler::RegisterCallback()
    {
        _dispatcher.sink<EventType>().connect<CallbackType>();
    }

    template <typename EventType, auto CallbackType, typename RegistrarType>
    void ECSEventHandler::UnregisterCallback(RegistrarType&& a_value_or_instance)
    {
        _dispatcher.sink<EventType>().disconnect<CallbackType>(std::forward<RegistrarType&&>(a_value_or_instance));
    }

    template <typename EventType, auto CallbackType>
    void ECSEventHandler::UnregisterCallback()
    {
        _dispatcher.sink<EventType>().disconnect<CallbackType>();
    }

    template <typename... EventType>
    bool ECSEventHandler::AreListenersOfTypeConnected()
    {
        return (!_dispatcher.sink<EventType>().empty() && ...);
    }

    template <typename... EventType>
    void ECSEventHandler::ProcessPendingEvents()
    {
        if constexpr (sizeof...(EventType) == 0)
            _dispatcher.update();
        else
            (_dispatcher.update<EventType>(), ...);
    }

    template <typename... EventType>
    void ECSEventHandler::ClearPendingEvents()
    {
        if constexpr (sizeof...(EventType) == 0)
            _dispatcher.clear();
        else
            (_dispatcher.clear<EventType>(), ...);
    }

#pragma endregion

} // End of namespace ~ DECS.
