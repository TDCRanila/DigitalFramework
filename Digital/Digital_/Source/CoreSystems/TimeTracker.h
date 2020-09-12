#pragma once

#include <chrono>

namespace DCore
{
    typedef float64 TimeUnit;
    
    TimeUnit SecondsToMilliseconds(const TimeUnit a_time);
    TimeUnit MillisecondsToSeconds(const TimeUnit a_time);
    
    class TimeTracker final
    {
    public:
        TimeTracker();
        ~TimeTracker();

        void StartTimer();
        void StopTimer();
        void ResetTimer(bool a_reset_and_start_timer);

        TimeUnit FetchTime() const;
        TimeUnit StopAndFetchTime();
        TimeUnit ResetAndFetchTime(bool a_reset_and_start_timer);

    private:
        std::chrono::high_resolution_clock::time_point start_time_point_;
        std::chrono::high_resolution_clock::time_point end_time_point_;

        TimeUnit current_delta_time_;

        bool is_tracking_time_;
    };

} // End of namespace ~ DCore.
