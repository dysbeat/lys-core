#pragma once

#include <boost/hana/experimental/type_name.hpp>
#include <boost/hana/fold.hpp>
#include <boost/hana/intersperse.hpp>
#include <boost/hana/string.hpp>
#include <fmt/format.h>
#include <array>
#include <string>
#include <string_view>
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
constexpr auto to_type = [](auto &&... x) { return T{std::forward<decltype(x)>(x)...}; };

template <typename T>
auto from_str(std::string str)
{
    if constexpr (std::is_same_v<int, T>)
    {
        return std::stoi(str);
    }
    else if constexpr (std::is_same_v<double, T>)
    {
        return str;
    }
    else if constexpr (std::is_same_v<std::string, T>)
    {
        return std::stod(str);
    }
}

template <typename Sep = decltype(boost::hana::string_c<>)>
constexpr auto join = [](auto && xs) {
    using namespace boost;
    return hana::fold(hana::intersperse(std::forward<decltype(xs)>(xs), Sep{}), hana::string_c<>, hana::_ + hana::_);
};

template <typename T>
const auto type_name = boost::hana::experimental::type_name<std::decay_t<T>>().c_str();

template <typename T>
const auto type_name_c = boost::hana::experimental::type_name<std::decay_t<T>>();

template <typename Fmt, typename... Args>
constexpr auto format(Fmt /*unused*/, Args... /*unused*/);

template <char Char1, char... Chars>
constexpr auto first_char()
{
    return Char1;
}

template <char Char1, char... Chars>
constexpr auto string_rest()
{
    return boost::hana::string<Chars...>{};
}

template <template <char...> typename Fmt, char... Chars>
constexpr auto format(Fmt<Chars...> fmt)
{
    return fmt;
}

template <template <char...> typename Fmt, char... Chars, typename Arg1, typename... Args>
constexpr auto format(Fmt<Chars...> /*unused*/, Arg1 arg1, Args... args)
{
    static_assert(sizeof...(Chars) > 0);
    constexpr auto c = first_char<Chars...>();
    if constexpr (c == '_')
    {
        return arg1 + format(string_rest<Chars...>(), args...);
    }
    else
    {
        return boost::hana::string<c>{} + format(string_rest<Chars...>(), arg1, args...);
    }
}

} // namespace lys::core::sql::helpers
