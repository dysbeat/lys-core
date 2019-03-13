#pragma once

#include <boost/hana/experimental/type_name.hpp>
#include <fmt/format.h>
#include <array>
#include <tuple>
#include <vector>

namespace lys::core::sql::helpers
{

template <typename T>
constexpr auto to_array = [](auto &&... x) { return std::array<T, sizeof...(x)>{std::forward<decltype(x)>(x)...}; };

template <typename T>
constexpr auto to_vector = [](auto &&... x) { return std::vector<T>{std::forward<decltype(x)>(x)...}; };

constexpr auto to_tuple = [](auto &&... x) { return std::make_tuple(std::forward<decltype(x)>(x)...); };

auto to_str = [](auto &&... x) {
    std::array<const char *, sizeof...(x)> array;
    std::fill(std::begin(array), std::end(array), "\"{}\"");
    return fmt::format(fmt::format("{}", fmt::join(array, ", ")), std::forward<decltype(x)>(x)...);
};

template <typename T>
const auto type_name = boost::hana::experimental::type_name<std::decay_t<T>>().c_str();

} // namespace lys::core::sql::helpers
