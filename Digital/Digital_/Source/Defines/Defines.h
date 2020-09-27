#pragma once

#include <iostream>
#include <limits>
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
	
	namespace DLimits
	{
		const float32 float32_min		= std::numeric_limits<float32>::min();
		const float32 float32_max		= std::numeric_limits<float32>::max();
		const float32 float32_epsilon	= std::numeric_limits<float32>::epsilon();

		const float64 float64_min		= std::numeric_limits<float64>::min();
		const float64 float64_max		= std::numeric_limits<float64>::max();
		const float64 float64_epsilon	= std::numeric_limits<float64>::epsilon();

		const int8  int8_min			= std::numeric_limits<uint8>::min();
		const int8  int8_max			= std::numeric_limits<uint8>::max();
		const int16 int16_min			= std::numeric_limits<uint16>::min();
		const int16 int16_max			= std::numeric_limits<uint16>::max();
		const int32 int32_min			= std::numeric_limits<uint32>::min();
		const int32 int32_max			= std::numeric_limits<uint32>::max();
		const int64 int64_min			= std::numeric_limits<uint64>::min();
		const int64 int64_max			= std::numeric_limits<uint64>::max();

		const uint8  uint8_min			= std::numeric_limits<uint8>::min();
		const uint8  uint8_max			= std::numeric_limits<uint8>::max();
		const uint16 uint16_min			= std::numeric_limits<uint16>::min();
		const uint16 uint16_max			= std::numeric_limits<uint16>::max();
		const uint32 uint32_min			= std::numeric_limits<uint32>::min();
		const uint32 uint32_max			= std::numeric_limits<uint32>::max();
		const uint64 uint64_min			= std::numeric_limits<uint64>::min();
		const uint64 uint64_max			= std::numeric_limits<uint64>::max();

	} // End of namespace DLimits

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

#include <Utility/TimeUtility.h>

#include <string>

	namespace 
	{
		const std::string DEBUG_LABEL	= "DEBUG";
		const std::string INFO_LABEL	= "INFO";
		const std::string WARN_LABEL	= "WARNING";
		const std::string ERROR_LABEL	= "ERROR";
	} // End of namespace

	#define __FILENAME__ (strrchr(__FILE__, DIR_SLASH_CHAR) ? strrchr(__FILE__, DIR_SLASH_CHAR) + 1 : __FILE__)

	#define DEBUGLOG(...) std::cout << DEBUG_LABEL	<< " | " << DUtility::GetTimeStamp() << " | " << __FILENAME__ << "(" << __LINE__ << "): "  << __VA_ARGS__ << std::endl;
	#define INFOLOG(...)  std::cout << INFO_LABEL	<< " | " << DUtility::GetTimeStamp() << " | " << __FILENAME__ << "(" << __LINE__ << "): "  << __VA_ARGS__ << std::endl;
	#define WARNLOG(...)  std::cout << WARN_LABEL	<< " | " << DUtility::GetTimeStamp() << " | " << __FILENAME__ << "(" << __LINE__ << "): "  << __VA_ARGS__ << std::endl;
	#define ERRORLOG(...) std::cout << ERROR_LABEL	<< " | " << DUtility::GetTimeStamp() << " | " << __FILENAME__ << "(" << __LINE__ << "): "  << __VA_ARGS__ << std::endl;

#elif defined(RELEASE64)
	#define DEBUGLOG(...)	{ /*EMPTY*/ }
	#define INFOLOG(...)	{ /*EMPTY*/ }
	#define WARNLOG(...)	{ /*EMPTY*/ }
	#define ERRORLOG(...)	{ /*EMPTY*/ }
#endif
