#pragma once

#include <type_traits>

template <class T>
constexpr typename std::underlying_type<T>::type to_underlying(T a_object) noexcept 
{
	return static_cast<typename std::underlying_type<T>::type>(a_object);
}

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
