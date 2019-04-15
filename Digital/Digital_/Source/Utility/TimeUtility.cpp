#include <Utility/TimeUtility.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace DUtility
{
	std::string GetTimeAndDate()
	{
		std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::ostringstream oss;
		oss << std::put_time(localtime(&t), "%F %T");
		return oss.str();
	}

	std::string GetTime()
	{
		std::string time_and_date = GetTimeAndDate();
		const auto begin_it(time_and_date.begin());
		const auto end_it(time_and_date.begin() + 11);
		time_and_date.erase(begin_it, end_it);
		return time_and_date;
	}

	std::string GetDate()
	{
		std::string time_and_date = GetTimeAndDate();
		const auto begin_it(time_and_date.begin() + 10);
		const auto end_it(time_and_date.end());
		time_and_date.erase(begin_it, end_it);
		return time_and_date;
	}

} // End of namespace ~ DUtility
