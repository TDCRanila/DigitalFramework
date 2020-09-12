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
