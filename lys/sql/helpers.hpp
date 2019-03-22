#pragma once

#include <boost/hana/fold.hpp>
#include <boost/hana/intersperse.hpp>
#include <boost/hana/string.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace std
{

template <typename T>
std::ostream & operator<<(std::ostream & os, optional<T> o)
{
    if (o)
    {
        return os << o;
    }
    return os << "NULL";
}

} // namespace std

namespace lys::core::sql::helpers
{

template <typename T>
constexpr auto to_array = [](auto &&... x) { return std::array<T, sizeof...(x)>{std::forward<decltype(x)>(x)...}; };

template <typename T>
constexpr auto to_vector = [](auto &&... x) { return std::vector<T>{std::forward<decltype(x)>(x)...}; };

constexpr auto to_tuple = [](auto &&... x) { return std::make_tuple(std::forward<decltype(x)>(x)...); };

template <size_t N, typename Str, typename Sep>
constexpr auto make_format(Str str, Sep sep)
{
    if constexpr (N > 0)
    {
        return str + sep + make_format<N - 1>(str, sep);
    }
    else
    {
        return str;
    }
}

template <typename T>
auto to_value(T && t)
{
    if constexpr (std::is_convertible_v<T, std::string>)
    {
        return fmt::format("\"{}\"", t);
    }
    else
    {
        return fmt::format("{}", t);
    }
}

auto to_str = [](auto &&... x) {
    using namespace boost::hana::literals;

    constexpr auto format = make_format<sizeof...(x) - 1>("{}"_s, ", "_s);
    return fmt::format(format.c_str(), to_value(std::forward<decltype(x)>(x))...);
};

template <typename T>
auto sanitize(T && t)
{
    if constexpr (std::is_same_v<std::decay_t<T>, const char *> || std::is_same_v<std::decay_t<T>, const unsigned char *>)
    {
        return (t == nullptr ? "" : t);
    }
    else
    {
        return t;
    }
}

template <typename T>
constexpr auto to_type = [](auto &&... x) { return T{sanitize(std::forward<decltype(x)>(x))...}; };

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

} // namespace lys::core::sql::helpers
