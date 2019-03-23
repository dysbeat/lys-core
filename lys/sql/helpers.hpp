#pragma once

#include <lys/str/join.hpp>
#include <lys/str/repeat.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <optional>

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

    constexpr auto format = str::join<str::comma_t>(str::repeat<sizeof...(x)>("{}"_s));
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
