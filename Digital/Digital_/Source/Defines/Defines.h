#pragma once

#include <iostream>
#include <limits>
#include <stdint.h>

// POD Definitions.
using float32	= float_t;
using float64	= double_t;
using int8		= int8_t;
using int16		= int16_t;
using int32		= int32_t;
using int64		= int64_t;
using uint8		= uint8_t;
using uint16	= uint16_t;
using uint32	= uint32_t;
using uint64	= uint64_t;

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

// stringify
#define DFW_STRINGIFY(a_argument) #a_argument
