#pragma once

#include <lys/str/convert.hpp>
#include <lys/str/traits.hpp>
#include <boost/hana/concat.hpp>
#include <boost/hana/count.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/remove_range.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/take_while.hpp>

namespace lys::str
{

template <char ToReplace, char... Chars, char... WithChars>
constexpr auto replace(boost::hana::string<Chars...> str, boost::hana::string<WithChars...> replace_with)
{
    using namespace boost;
    if constexpr (!hana::count(str, ToReplace))
    {
        return str;
    }
    else
    {
        constexpr auto str_tuple      = from_hana_string(str);
        constexpr auto first_part     = hana::take_while(str_tuple, is_not_same_char<ToReplace>);
        constexpr auto to_remove_size = hana::size(first_part) + hana::size_c<1>;
        constexpr auto second_part    = hana::remove_range(str_tuple, hana::size_c<0>, to_remove_size);
        return to_hana_string(                                            //
            hana::concat(                                                 //
                hana::concat(first_part, from_hana_string(replace_with)), //
                second_part));
    }
}

} // namespace lys::str