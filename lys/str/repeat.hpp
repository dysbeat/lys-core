#pragma once

#include <boost/hana/range.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/tuple.hpp>

namespace lys::str
{

template <size_t N>
constexpr auto repeat = [](auto && str) { //
    return boost::hana::transform(boost::hana::to_tuple(boost::hana::range_c<int, 0, N>), [&](auto /*unused*/) { return str; });
};
}
