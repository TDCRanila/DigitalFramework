#pragma once

#include <Defines/TimeDefines.h>

#include <string>

namespace DUtility
{
	DCore::TimeUnit SecondsToMilliseconds(const DCore::TimeUnit a_time);
	DCore::TimeUnit MillisecondsToSeconds(const DCore::TimeUnit a_time);

	std::string GetTimeAndDateStamp();

	std::string GetTimeStamp();

	std::string GetDateStamp();

} // End of namespace ~ DUtility
