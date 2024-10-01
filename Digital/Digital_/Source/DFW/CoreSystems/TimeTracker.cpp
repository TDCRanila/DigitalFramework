#include <CoreSystems\TimeTracker.h>

namespace DFW
{
    TimeTracker::TimeTracker(bool a_start_timer) 
        : _is_tracking_time(false)
        , _start_time_point(std::chrono::nanoseconds(0))
        , _end_time_point(std::chrono::nanoseconds(0))
        , _current_elapsed_time(0.0f)
    {
        if (a_start_timer)
            StartTimer();
    }

    void TimeTracker::StartTimer()
    {
        if (!IsRunning())
        {
            _is_tracking_time   = true;
            _start_time_point   = std::chrono::high_resolution_clock::now();
        }
    }

    void TimeTracker::StopTimer()
    {
        if (IsRunning())
        {
            _is_tracking_time = false;
            _end_time_point = std::chrono::high_resolution_clock::now();

            _current_elapsed_time = std::chrono::duration_cast<std::chrono::duration<TimeUnit>>(_end_time_point - _start_time_point).count();
        }
    }

    void TimeTracker::ResetTimer(bool a_reset_and_start_timer)
    {
        _is_tracking_time = false;

        _start_time_point   = std::chrono::high_resolution_clock::time_point(std::chrono::nanoseconds(0));
        _end_time_point     = std::chrono::high_resolution_clock::time_point(std::chrono::nanoseconds(0));
        _current_elapsed_time = TimeUnit(0.0f);

        if (a_reset_and_start_timer)
        {
            StartTimer();
        }
    }

    bool TimeTracker::IsRunning() const
    {
        return _is_tracking_time;
    }

    TimeUnit TimeTracker::FetchElapsedTime() const
    {
        TimeUnit delta_time(0.0f);
        if (_is_tracking_time)
        {
            delta_time = std::chrono::duration_cast<std::chrono::duration<TimeUnit>>(std::chrono::high_resolution_clock::now() - _start_time_point).count();
        }
        else
        {
            delta_time = _current_elapsed_time;
        }

        return delta_time;
    }

    TimeUnit TimeTracker::StopAndFetchElapsedTime()
    {
        StopTimer();
        return FetchElapsedTime();
    }

    TimeUnit TimeTracker::ResetAndFetchElapsedTime(bool a_reset_and_start)
    {
        TimeUnit const time = FetchElapsedTime();
        ResetTimer(a_reset_and_start);
        return time;
    }

} // End of namespace ~ DFW.
