#pragma once

#include <iostream>

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
#define UNUSED(variable) (void)(variable)

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

	namespace {
		static inline std::string GetTime() {
			std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::ostringstream oss;
			oss << std::put_time(localtime(&t), "%F %T");
			return oss.str();

		}

		const std::string g_info_label	= "INFO ";
		const std::string g_debug_label	= "DEBUG";
		const std::string g_error_label	= "ERROR";

	} // End of namespace

	#define __FILENAME__ (strrchr(__FILE__, DIR_SLASH_CHAR) ? strrchr(__FILE__, DIR_SLASH_CHAR) + 1 : __FILE__)

	#define INFOLOG(message)  std::cout << g_info_label		<< " | " << __FILENAME__	<< "(" << __LINE__ << "): "  << #message << std::endl;
	#define DEBUGLOG(message) std::cout << g_debug_label	<< " | " << __FILENAME__	<< "(" << __LINE__ << "): "  << #message << std::endl;
	#define ERRORLOG(message) std::cout << g_error_label	<< " | " << __FILENAME__	<< "(" << __LINE__ << "): "  << #message << std::endl;

#elif defined(RELEASE64)
	#define INFOLOG(message)  { /*EMPTY*/ }
	#define DEBUGLOG(message) { /*EMPTY*/ }
	#define ERRORLOG(message) { /*EMPTY*/ }
#endif
