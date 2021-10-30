#include <Utility/TimeUtility.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace DUtility
{
	DCore::TimeUnit SecondsToMilliseconds(const DCore::TimeUnit a_time)
	{
		return a_time * 1000;
	}

	DCore::TimeUnit MillisecondsToSeconds(const DCore::TimeUnit a_time)
	{
		return a_time * 0.001f;
	}

	std::string GetTimeAndDateStamp()
	{
		std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::ostringstream oss;
		tm time_struct;
		localtime_s(&time_struct, &t);
		oss << std::put_time(&time_struct, "%F-%T");
		return oss.str();
	}

	std::string GetTimeStamp()
	{
		std::string time_and_date = GetTimeAndDateStamp();
		const auto begin_it(time_and_date.begin());
		const auto end_it(time_and_date.begin() + 11);
		time_and_date.erase(begin_it, end_it);
		return time_and_date;
	}

	std::string GetDateStamp()
	{
		std::string time_and_date = GetTimeAndDateStamp();
		const auto begin_it(time_and_date.begin() + 10);
		const auto end_it(time_and_date.end());
		time_and_date.erase(begin_it, end_it);
		return time_and_date;
	}

} // End of namespace ~ DUtility
