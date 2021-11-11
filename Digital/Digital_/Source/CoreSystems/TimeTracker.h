#pragma once

#include <Defines/TimeDefines.h>

#include <chrono>

namespace DFW
{   
    class TimeTracker final
    {
    public:
        TimeTracker(bool a_start_timer = false);
        ~TimeTracker();

        void StartTimer();
        void StopTimer();
        void ResetTimer(bool a_reset_and_start_timer);
        bool IsRunning() const;

        TimeUnit FetchElapsedTime() const;
        TimeUnit StopAndFetchElapsedTime();
        TimeUnit ResetAndFetchElapsedTime(bool a_reset_and_start_timer);

    private:
        std::chrono::high_resolution_clock::time_point _start_time_point;
        std::chrono::high_resolution_clock::time_point _end_time_point;

        TimeUnit _current_elapsed_time;

        bool _is_tracking_time;
    };

} // End of namespace ~ DFW.
