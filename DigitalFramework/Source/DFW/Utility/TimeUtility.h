#pragma once

#include <DFW/Defines/TimeDefines.h>

#include <string>

namespace DFW
{
	namespace DUtility
	{
		TimeUnit SecondsToMilliseconds(TimeUnit const a_time);
		TimeUnit MillisecondsToSeconds(TimeUnit const a_time);

		std::string GetTimeAndDateStamp();

		std::string GetTimeStamp();

		std::string GetDateStamp();

	} // End of namespace ~ DUtility.

} // End of namespace ~ DFW.
