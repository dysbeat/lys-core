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

} // namespace lys::core