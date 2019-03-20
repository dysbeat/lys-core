#pragma once

#include <boost/hana/plus.hpp>
#include <boost/hana/remove_range.hpp>
#include <boost/hana/reverse.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/take_while.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

namespace lys::core::sql::helpers
{

template <char... Chars>
constexpr auto to_hana_string(boost::hana::tuple<boost::hana::char_<Chars>...> /*unused*/)
{
    return boost::hana::string<Chars...>{};
}

template <char... Chars, template <char...> typename HanaString>
constexpr auto to_hana_tuple(HanaString<Chars...> str)
{
    return boost::hana::unpack(str, boost::hana::make_tuple);
}

template <char ToReplace, char... Chars, char... WithChars>
constexpr auto replace(boost::hana::string<Chars...> str, boost::hana::string<WithChars...> replace_with)
{
    using namespace boost;
    constexpr auto str_tuple          = to_hana_tuple(str);
    constexpr auto replace_with_tuple = to_hana_tuple(replace_with);

    constexpr auto first_part = hana::take_while(str_tuple, [](auto x) {
        if constexpr (!std::is_same_v<decltype(x), hana::char_<ToReplace>>)
        {
            return hana::true_c;
        }
        else
        {
            return hana::false_c;
        }
    });

    if constexpr (hana::size(first_part) == hana::size(str_tuple))
    {
        return str;
    }
    else
    {
        constexpr auto to_remove_size = hana::size(first_part) + hana::size_c<1>;
        constexpr auto second_part    = hana::remove_range(str_tuple, hana::size_c<0>, to_remove_size);

        return to_hana_string(hana::concat(hana::concat(first_part, replace_with_tuple), second_part));
    }
}

template <char BeforeThis, char... Chars>
constexpr auto remove_before(boost::hana::string<Chars...> str)
{
    using namespace boost;
    return to_hana_string(hana::reverse(hana::take_while(hana::reverse(to_hana_tuple(str)), [](auto x) {
        if constexpr (!std::is_same_v<decltype(x), hana::char_<BeforeThis>>)
        {
            return hana::true_c;
        }
        else
        {
            return hana::false_c;
        }
    })));
}

} // namespace lys::core::sql::helpers