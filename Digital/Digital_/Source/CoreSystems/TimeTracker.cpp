#include <CoreSystems\TimeTracker.h>

#include <chrono>

namespace DCore
{
    TimeUnit SecondsToMilliseconds(const TimeUnit a_time)
    {
        return a_time * 1000;
    }

    TimeUnit MillisecondsToSeconds(const TimeUnit a_time)
    {
        return a_time * 0.001f;
    }

    TimeTracker::TimeTracker() :
        is_tracking_time_(false),
        start_time_point_(std::chrono::nanoseconds(0)),
        end_time_point_(std::chrono::nanoseconds(0)),
        current_delta_time_(0.0f)
    {
    }

    TimeTracker::~TimeTracker()
    {
    }

    void TimeTracker::StartTimer()
    {
        is_tracking_time_   = true;
        start_time_point_   = std::chrono::high_resolution_clock::now();
    }

    void TimeTracker::StopTimer()
    {
        is_tracking_time_   = false;
        end_time_point_     = std::chrono::high_resolution_clock::now();

        current_delta_time_ = std::chrono::duration_cast<std::chrono::duration<TimeUnit>>(end_time_point_ - start_time_point_).count();
    }

    void TimeTracker::ResetTimer(bool a_reset_and_start_timer)
    {
        is_tracking_time_ = false;

        start_time_point_   = std::chrono::high_resolution_clock::time_point(std::chrono::nanoseconds(0));
        end_time_point_     = std::chrono::high_resolution_clock::time_point(std::chrono::nanoseconds(0));
        current_delta_time_ = TimeUnit(0.0f);

        if (a_reset_and_start_timer)
        {
            StartTimer();
        }
    }

    TimeUnit TimeTracker::FetchTime() const
    {
        TimeUnit delta_time(0.0f);
        if (is_tracking_time_)
        {
            delta_time = std::chrono::duration_cast<std::chrono::duration<TimeUnit>>(std::chrono::high_resolution_clock::now() - start_time_point_).count();
        }
        else
        {
            delta_time = current_delta_time_;
        }

        return delta_time;
    }

    TimeUnit TimeTracker::StopAndFetchTime()
    {
        StopTimer();
        return FetchTime();
    }

    TimeUnit TimeTracker::ResetAndFetchTime(bool a_reset_and_start)
    {
        const TimeUnit time = FetchTime();
        ResetTimer(a_reset_and_start);
        return time;
    }

} // End of namespace ~ DCore.
