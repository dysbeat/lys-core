#pragma once

#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>

namespace lys::str
{

template <char... Chars>
constexpr auto to_hana_string(boost::hana::tuple<boost::hana::char_<Chars>...> /*unused*/)
{
    return boost::hana::string<Chars...>{};
}

template <char... Chars, template <char...> typename HanaString>
constexpr auto from_hana_string(HanaString<Chars...> str)
{
    return boost::hana::unpack(str, boost::hana::make_tuple);
}

} // namespace lys::str
