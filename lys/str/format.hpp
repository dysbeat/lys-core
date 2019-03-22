#pragma once

#include <lys/str/replace.hpp>
#include <boost/hana/count.hpp>
#include <boost/hana/fold.hpp>

namespace lys::str
{

template <char Selector>
constexpr auto format = [](auto fmt_str, auto... xs) {
    using namespace boost;
    constexpr auto selector_count = hana::count(fmt_str, Selector);
    constexpr auto arg_count      = std::tuple_size_v<decltype(std::make_tuple(xs...))>;
    static_assert(selector_count == arg_count, "Mismatch between selector count and number of replacing strings");
    // constexpr auto first  = replace<Selector>(fmt_str, x1);
    return hana::fold(hana::make_tuple(fmt_str, xs...), [](auto f, auto x) { return replace<Selector>(f, x); });
};
}
