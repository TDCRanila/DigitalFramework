#pragma once

#include <Defines/Defines.h>

#include <limits>

namespace MD
{

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
