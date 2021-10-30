#pragma once

#include <Defines/Defines.h>

#include <limits>

namespace DMath
{
	// Backup Hardcoded Limits.
	const float32 float32_min	= std::numeric_limits<float32>::min();
	const float32 float32_max	= std::numeric_limits<float32>::max();

	const float64 float64_min	= std::numeric_limits<float64>::min();
	const float64 float64_max	= std::numeric_limits<float64>::max();

	const int8  int8_min	= std::numeric_limits<int8>::min();
	const int8  int8_max	= std::numeric_limits<int8>::max();
	const int16 int16_min	= std::numeric_limits<int16>::min();
	const int16 int16_max	= std::numeric_limits<int16>::max();
	const int32 int32_min	= std::numeric_limits<int32>::min();
	const int32 int32_max	= std::numeric_limits<int32>::max();
	const int64 int64_min	= std::numeric_limits<int64>::min();
	const int64 int64_max	= std::numeric_limits<int64>::max();

	const uint8  uint8_min	= std::numeric_limits<uint8>::min();
	const uint8  uint8_max	= std::numeric_limits<uint8>::max();
	const uint16 uint16_min = std::numeric_limits<uint16>::min();
	const uint16 uint16_max = std::numeric_limits<uint16>::max();
	const uint32 uint32_min = std::numeric_limits<uint32>::min();
	const uint32 uint32_max = std::numeric_limits<uint32>::max();
	const uint64 uint64_min = std::numeric_limits<uint64>::min();
	const uint64 uint64_max = std::numeric_limits<uint64>::max();

	// Epsilon.
	const float32 FLT32_EPSILON = std::numeric_limits<float32>::epsilon();
	const float64 FLT64_EPSILON = std::numeric_limits<float64>::epsilon();

	// Pi definiton.
	const float32 PI		= 3.14159f;
	const float64 PI_DOUBLE	= 3.14159265358979323846f;

	// Degree <-> Rad Conversion.
	static inline float32 DegToRad(float32 a_degrees)
	{
		return (PI / 180) * a_degrees;
	}

	static inline float32 RadToDeg(float32 a_radians)
	{
		return (180 / PI) * a_radians;
	}

	static inline float64 DegToRad(float64 a_degrees)
	{
		return (PI_DOUBLE / 180) * a_degrees;
	}

	static inline float64 RadToDeg(float64 a_radians)
	{
		return (180 / PI_DOUBLE) * a_radians;
	}

	// Numeric Limits of type T.
	template<class T>
	static inline  T GetMinValueOfType()
	{
		return std::numeric_limits<T>::min();
	}

	template<class T>
	static inline T GetMaxValueOfType()
	{
		return std::numeric_limits<T>::max();
	}

} // End of namespace ~ MD
