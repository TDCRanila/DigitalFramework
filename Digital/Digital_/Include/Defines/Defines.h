#pragma once

#include <iostream>
#include <stdint.h>

// POD Typedefs
namespace 
{
	typedef float_t		float32;
	typedef double_t	float64;
	
	typedef int8_t		int8;
	typedef int16_t		int16;
	typedef int32_t		int32;
	typedef int64_t		int64;
	
	typedef uint8_t		uint8;
	typedef uint16_t	uint16;
	typedef uint32_t	uint32;
	typedef uint64_t	uint64;

} // End of namespace

// Debug Assert Defines
#if defined(DEBUG64) || defined(PRERELEASE64)
	#include <assert.h>
	#define DASSERT(...) assert(__VA_ARGS__)
#elif defined(RELEASE64)
	#define DASSERT(...) { /*EMPTY*/ }
#endif

// Directory Slashes Defines
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define DIR_SLASH_CHAR	'\\'
	#define DIR_SLASH		"\\"
#else
	#define DIR_SLASH_CHAR	'/'
	#define DIR_SLASH		"/"
#endif

// Unused Variable Define
#define UNUSED(a_variable) (void)(a_variable)

// Windows Min & Max Undefs
#if defined(min) && defined(max)
	#undef min
	#undef max
#endif

// Logging Defines
#if defined(DEBUG64) || defined(PRERELEASE64)

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

	namespace 
	{
		static inline std::string GetTimeAndDate() 
		{
			std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::ostringstream oss;
			oss << std::put_time(localtime(&t), "%F %T");
			return oss.str();
		}

		static inline std::string GetTime()
		{
			std::string time_and_date = GetTimeAndDate();
			const auto begin_it(time_and_date.begin());
			const auto end_it(time_and_date.begin() + 11);
			time_and_date.erase(begin_it, end_it);
			return time_and_date;
		}

		static inline std::string GetDate()
		{
			std::string time_and_date = GetTimeAndDate();
			const auto begin_it(time_and_date.begin() + 10);
			const auto end_it(time_and_date.end());
			time_and_date.erase(begin_it, end_it);
			return time_and_date;
		}

		const std::string INFO_LABEL	= "INFO";
		const std::string DEBUG_LABEL	= "DEBUG";
		const std::string ERROR_LABEL	= "ERROR";

	} // End of namespace

	#define __FILENAME__ (strrchr(__FILE__, DIR_SLASH_CHAR) ? strrchr(__FILE__, DIR_SLASH_CHAR) + 1 : __FILE__)

	#define INFOLOG(a_message)  std::cout << INFO_LABEL		<< " | " << __FILENAME__	<< "(" << __LINE__ << "): "  << #a_message << std::endl;
	#define DEBUGLOG(a_message) std::cout << DEBUG_LABEL	<< " | " << __FILENAME__	<< "(" << __LINE__ << "): "  << #a_message << std::endl;
	#define ERRORLOG(a_message) std::cout << ERROR_LABEL	<< " | " << __FILENAME__	<< "(" << __LINE__ << "): "  << #a_message << std::endl;

#elif defined(RELEASE64)
	#define INFOLOG(a_message)  { /*EMPTY*/ }
	#define DEBUGLOG(a_message) { /*EMPTY*/ }
	#define ERRORLOG(a_message) { /*EMPTY*/ }
#endif
