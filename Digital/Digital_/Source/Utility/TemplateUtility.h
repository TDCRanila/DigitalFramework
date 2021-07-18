#pragma once

#include <type_traits>

template <typename T>
constexpr typename std::underlying_type<T>::type to_underlying(T a_object) noexcept 
{
	return static_cast<typename std::underlying_type<T>::type>(a_object);
}

template <typename T>
struct always_false : std::false_type
{
	/*EMPTY*/
};

template <typename T>
concept IsAlwaysFalse = always_false<T>::value;

template <typename Ta, typename Tb>
struct is_same : std::false_type
{
	/*EMPTY*/
};

template <typename T>
struct is_same<T, T> : std::true_type
{
	/*EMPTY*/
};

template <typename Ta, typename Tb>
concept AreSameTypes = std::is_same_v<Ta, Tb>;

template <typename Derived, typename Base>
struct is_derived_from : std::bool_constant<std::is_base_of_v<Base, Derived> && !std::is_same_v<Base, Derived>>
{
	/*EMPTY*/
};

template <typename Derived, typename Base>
concept IsDerivedFrom = is_derived_from<Derived, Base>::value;

template <typename Derived, typename Base>
concept IsBasedOf = std::is_base_of_v<Base, Derived>;
