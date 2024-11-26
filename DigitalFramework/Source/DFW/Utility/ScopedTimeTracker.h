#pragma once

#include <DFW/CoreSystems/TimeTracker.h>
#include <DFW/CoreSystems/Logging/Logger.h>

#include <string>

namespace DFW
{
    namespace DUtility
    {
        class ScopedTimeTracker final
        {
        public:
            ScopedTimeTracker(std::string_view a_name)
                : _time_tracker(true)
                , _time_tracker_name(a_name)
            {
            }

            ~ScopedTimeTracker()
            {
                TimeUnit const resulted_time = _time_tracker.StopAndFetchElapsedTime();
                DFW_INFOLOG("{} - Elapsed Time: {}", _time_tracker_name, resulted_time);
            }

        private:
            TimeTracker _time_tracker;
            std::string const _time_tracker_name;
        };

    } // End of namespace ~ DUtility.

} // End of namespace ~ DFW.
