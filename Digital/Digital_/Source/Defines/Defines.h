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

// std::bind replacement macro
#define DFW_BIND_FUNC(func) [this](auto&&... a_args) -> decltype(auto) { return this->func(std::forward<decltype(a_args)>(a_args)...); }
