#pragma once

#include <iostream>
#include <limits>
#include <stdint.h>

// POD Typedefs
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

// Debug Assert Defines
#if defined(DFW_PDEBUG64) || defined(DFW_DEBUG64) || defined(DFW_PRERELEASE64)
	#include <assert.h>
	#define DFW_ASSERT(...) assert(__VA_ARGS__)
#elif defined(DFW_RELEASE64) || defined(DFW_FINAL64)
	#define DFW_ASSERT(...) { /*EMPTY*/ }
#endif

// Directory Slashes Defines
#if defined(DFW_PLATFORM_WINDOWS) && defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(DFW_PLATFORM_UNIX) && !defined(__CYGWIN__)
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
#if defined(DFW_PDEBUG64) || defined(DFW_DEBUG64) || defined(DFW_PRERELEASE64)

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

	#define DFW_DEBUGLOG(...) std::cout << DEBUG_LABEL	<< " | " << DUtility::GetTimeStamp() << " | " << __FILENAME__ << "(" << __LINE__ << "): "  << __VA_ARGS__ << std::endl;
	#define DFW_INFOLOG(...)  std::cout << INFO_LABEL	<< " | " << DUtility::GetTimeStamp() << " | " << __FILENAME__ << "(" << __LINE__ << "): "  << __VA_ARGS__ << std::endl;
	#define DFW_WARNLOG(...)  std::cout << WARN_LABEL	<< " | " << DUtility::GetTimeStamp() << " | " << __FILENAME__ << "(" << __LINE__ << "): "  << __VA_ARGS__ << std::endl;
	#define DFW_ERRORLOG(...) std::cout << ERROR_LABEL	<< " | " << DUtility::GetTimeStamp() << " | " << __FILENAME__ << "(" << __LINE__ << "): "  << __VA_ARGS__ << std::endl;

#elif defined(DFW_RELEASE64) || defined(DFW_FINAL64)
	#define DFW_DEBUGLOG(...)	{ /*EMPTY*/ }
	#define DFW_INFOLOG(...)	{ /*EMPTY*/ }
	#define DFW_WARNLOG(...)	{ /*EMPTY*/ }
	#define DFW_ERRORLOG(...)	{ /*EMPTY*/ }
#endif
