#pragma once

#include <Defines/Defines.h>

#include <limits>

namespace MD
{

	// Pi definiton.
	const float32 pi		= 3.14159f;
	const float64 pi_double	= 3.14159265358979323846f;

	// Degree <-> Rad Conversion.
	static inline float32 DegToRad(float32 a_degrees)
	{
		return (pi / 180) * a_degrees;
	}

	static inline float32 RadToDeg(float32 a_radians)
	{
		return (180 / pi) * a_radians;
	}

	static inline float64 DegToRad(float64 a_degrees)
	{
		return (pi_double / 180) * a_degrees;
	}

	static inline float64 RadToDeg(float64 a_radians)
	{
		return (180 / pi_double) * a_radians;
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
