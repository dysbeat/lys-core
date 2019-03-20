#pragma once

#include <optional>

namespace lys::core::sql
{

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

template <typename T>
struct is_optional : std::false_type
{};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type
{};

template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

template <typename T>
struct is_entry : std::false_type
{};

template <typename T>
constexpr bool is_entry_v = is_entry<T>::value;

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

} // namespace lys::core