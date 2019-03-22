#pragma once

#include <lys/str/convert.hpp>
#include <lys/str/traits.hpp>
#include <boost/hana/reverse.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/take_while.hpp>

namespace lys::str
{

template <char BeforeThis, char... Chars>
constexpr auto remove_before(boost::hana::string<Chars...> str)
{
    using namespace boost;
    return to_hana_string(                            //
        hana::reverse(                                //
            hana::take_while(                         //
                hana::reverse(from_hana_string(str)), //
                is_not_same_char<BeforeThis>)));
}

} // namespace lys::str
