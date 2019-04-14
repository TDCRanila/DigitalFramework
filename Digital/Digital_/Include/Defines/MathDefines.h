#pragma once

#include <Defines/Defines.h>

#include <limits>

namespace 
{

	// Pi definiton.
	const float pi			= 3.14159f;
	const double pi_double	= 3.14159265358979323846f;

	// Degree <-> Rad Conversion.
	static inline float DegToRad(float a_degrees) 
	{
		return (pi / 180) * a_degrees;
	}

	static inline float RadToDeg(float a_radians) 
	{
		return (180 / pi) * a_radians;
	}

	static inline float DegToRad(double a_degrees) 
	{
		return (pi_double / 180) * a_degrees;
	}

	static inline float RadToDeg(double a_radians) 
	{
		return (180 / pi_double) * a_radians;
	}

	// Numeric Limits.
	static inline float GetMinFloat() 
	{
		return std::numeric_limits<float>::min();
	}

	static inline float GetMaxFloat() 
	{
		return std::numeric_limits<float>::max();
	}

	static inline double GetMinDouble() 
	{
		return std::numeric_limits<double>::min();
	}

	static inline double GetMaxDouble() 
	{
		return std::numeric_limits<double>::max();
	}

} // End of namespace
