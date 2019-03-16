#pragma once

#include <lys/sql/types.hpp>

template <typename T>
struct is_optional : std::false_type
{};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type
{};

template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

namespace lys::core
{

template <typename T>
struct underlying_type
{
    using type = T;
};

template <typename T>
struct underlying_type<std::optional<T>>
{
    using type = T;
};

template <typename T>
struct underlying_type<entry<T>>
{
    using type = int;
};

template <typename T>
using underlying_type_t = typename underlying_type<T>::type;

template <typename T>
struct is_entry : std::conditional_t<std::is_base_of_v<entry<T>, T>, std::true_type, std::false_type>
{};

template <typename T>
constexpr bool is_entry_v = is_entry<T>::value;

template <class T>
struct get_member_pointer_base;

template <class T, class U>
struct get_member_pointer_base<T U::*>
{
    using type = U;
};

template <typename T>
using get_member_pointer_base_t = typename get_member_pointer_base<T>::type;

template <class T>
struct get_member_pointed_to;

template <class T, class U>
struct get_member_pointed_to<T U::*>
{
    using type = T;
};

template <typename T>
using get_member_pointed_to_t = typename get_member_pointed_to<T>::type;

} // namespace lys::core