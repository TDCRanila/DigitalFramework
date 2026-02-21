#include <DFW/Utility/TimeUtility.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace DFW
{
	namespace DUtility
	{
		DFW::TimeUnit SecondsToMilliseconds(const DFW::TimeUnit a_time)
		{
			return a_time * 1000;
		}

		DFW::TimeUnit MillisecondsToSeconds(const DFW::TimeUnit a_time)
		{
			return a_time * 0.001f;
		}

		std::string GetTimeAndDateStamp()
		{
			std::time_t t = time(nullptr);
			std::ostringstream oss;
			oss << std::put_time(std::localtime(&t), "%F-%H%M%S");
			return oss.str();
		}

		std::string GetTimeStamp()
		{
			std::string time_and_date = GetTimeAndDateStamp();
			auto const& begin_it(time_and_date.begin());
			auto const& end_it(time_and_date.begin() + 11);
			time_and_date.erase(begin_it, end_it);
			return time_and_date;
		}

		std::string GetDateStamp()
		{
			std::string time_and_date = GetTimeAndDateStamp();
			auto const& begin_it(time_and_date.begin() + 10);
			auto const& end_it(time_and_date.end());
			time_and_date.erase(begin_it, end_it);
			return time_and_date;
		}

	} // End of namespace ~ DUtility.

} // End of namespace ~ DFW.
