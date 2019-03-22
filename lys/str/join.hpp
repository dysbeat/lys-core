#pragma once

#include <boost/hana/fold.hpp>
#include <boost/hana/intersperse.hpp>
#include <boost/hana/string.hpp>

namespace lys::str
{

template <typename Sep = decltype(boost::hana::string_c<>)>
constexpr auto join = [](auto && xs) {
    using namespace boost;
    return hana::fold(hana::intersperse(std::forward<decltype(xs)>(xs), Sep{}), hana::string_c<>, hana::_ + hana::_);
};

constexpr auto space = boost::hana::string_c<' '>;
using space_t        = decltype(space);

constexpr auto comma = boost::hana::string_c<','>;
using comma_t        = decltype(comma);

} // namespace lys::str
