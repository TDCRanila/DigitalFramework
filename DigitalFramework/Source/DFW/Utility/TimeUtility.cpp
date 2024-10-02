#include <Utility/TimeUtility.h>

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
			std::time_t const t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::ostringstream oss;
			tm time_struct;
			localtime_s(&time_struct, &t);
			oss << std::put_time(&time_struct, "%F-%T");
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
