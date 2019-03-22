#pragma once

// #include <boost/hana/char.hpp>
#include <boost/hana/not.hpp>

namespace lys::str
{

template <char Char>
constexpr auto is_same_char = [](auto x) { return std::is_same<decltype(x), boost::hana::char_<Char>>{}; };

template <char Char>
constexpr auto is_not_same_char = [](auto x) { return boost::hana::not_(is_same_char<Char>(x)); };

} // namespace lys::str
