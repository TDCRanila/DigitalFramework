#pragma once

namespace 
{
	template <class T>
	struct always_false : std::false_type
	{
		/*EMPTY*/
	};

	template <class Ta, class Tb>
	struct is_same : std::false_type
	{
		/*EMPTY*/
	};

	template <class T>
	struct is_same<T, T> : std::true_type
	{
		/*EMPTY*/
	};

}
