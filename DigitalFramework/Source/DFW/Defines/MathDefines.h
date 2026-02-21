#pragma once

#include <DFW/Defines/Defines.h>

#include <limits>

namespace DFW
{
	namespace DMath
	{
		// Backup Hardcoded Limits.
		constexpr float32 float32_min = std::numeric_limits<float32>::min();
		constexpr float32 float32_max = std::numeric_limits<float32>::max();

		constexpr float64 float64_min = std::numeric_limits<float64>::min();
		constexpr float64 float64_max = std::numeric_limits<float64>::max();

		constexpr int8  int8_min = std::numeric_limits<int8>::min();
		constexpr int8  int8_max = std::numeric_limits<int8>::max();
		constexpr int16 int16_min = std::numeric_limits<int16>::min();
		constexpr int16 int16_max = std::numeric_limits<int16>::max();
		constexpr int32 int32_min = std::numeric_limits<int32>::min();
		constexpr int32 int32_max = std::numeric_limits<int32>::max();
		constexpr int64 int64_min = std::numeric_limits<int64>::min();
		constexpr int64 int64_max = std::numeric_limits<int64>::max();

		constexpr uint8  uint8_min = std::numeric_limits<uint8>::min();
		constexpr uint8  uint8_max = std::numeric_limits<uint8>::max();
		constexpr uint16 uint16_min = std::numeric_limits<uint16>::min();
		constexpr uint16 uint16_max = std::numeric_limits<uint16>::max();
		constexpr uint32 uint32_min = std::numeric_limits<uint32>::min();
		constexpr uint32 uint32_max = std::numeric_limits<uint32>::max();
		constexpr uint64 uint64_min = std::numeric_limits<uint64>::min();
		constexpr uint64 uint64_max = std::numeric_limits<uint64>::max();

		// Epsilon.
		constexpr float32 FLT32_EPSILON = std::numeric_limits<float32>::epsilon();
		constexpr float64 FLT64_EPSILON = std::numeric_limits<float64>::epsilon();

		// Pi definiton.
		constexpr float32 PI = 3.14159f;
		constexpr float64 PI_DOUBLE = 3.14159265358979323846f;

		// Degree <-> Rad Conversion.
		constexpr inline float32 DegToRad(float32 a_degrees)
		{
			return (PI / 180) * a_degrees;
		}

		constexpr inline float32 RadToDeg(float32 a_radians)
		{
			return (180 / PI) * a_radians;
		}

		constexpr inline float64 DegToRad(float64 a_degrees)
		{
			return (PI_DOUBLE / 180) * a_degrees;
		}

		constexpr inline float64 RadToDeg(float64 a_radians)
		{
			return (180 / PI_DOUBLE) * a_radians;
		}

		// Numeric Limits of type T.
		template<class T>
		constexpr inline  T GetMinValueOfType()
		{
			return std::numeric_limits<T>::min();
		}

		template<class T>
		constexpr inline T GetMaxValueOfType()
		{
			return std::numeric_limits<T>::max();
		}

	} // End of namespace ~ DMath.

} // End of namespace ~ DFW.
