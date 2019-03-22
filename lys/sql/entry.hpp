#pragma once

#include <lys/sql/converters.hpp>
#include <lys/sql/helpers.hpp>
#include <lys/sql/traits.hpp>
#include <lys/str/join.hpp>
#include <boost/hana/adapt_struct.hpp>
#include <boost/hana/drop_back.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/members.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/zip.hpp>
#include <functional>

namespace lys::core::sql
{

using id_type = int;

template <typename T>
struct entry : T
{
    using base_type = T;
    id_type id;
};

template <typename T>
constexpr auto entry_name{BOOST_HANA_STRING("")};

template <typename T>
struct underlying_type<sql::entry<T>>
{
    using type = int;
};

template <typename T>
using underlying_type_t = typename underlying_type<T>::type;

template <typename T>
struct entry_helper
{
    using entry_type = entry<T>;

private:
    static constexpr auto drop_last  = [](auto x) { return boost::hana::drop_back(x, boost::hana::int_c<1>); };
    static constexpr auto drop_first = [](auto x) { return boost::hana::drop_front(x, boost::hana::int_c<1>); };

public:
    static constexpr auto name{entry_name<T>};

    static constexpr auto entry_accessors = boost::hana::accessors<entry_type>();
    // accessors are used without the id (which appears last) in most cases
    static constexpr auto accessors = drop_last(entry_accessors);

    static constexpr auto entry_members = [](auto && t) { return boost::hana::members(entry_type{t}); };
    static constexpr auto members       = [](auto && t) { return drop_last(boost::hana::members(entry_type{t})); };

    static constexpr auto entry_keys = boost::hana::concat(
        boost::hana::make_tuple(BOOST_HANA_STRING("id")), boost::hana::transform(accessors, [](auto x) { return boost::hana::first(x); }));
    static constexpr auto keys = drop_first(entry_keys);

    static constexpr auto entry_sql_calls =
        boost::hana::concat(boost::hana::make_tuple(sqlite3_column_int), boost::hana::transform(accessors, [](auto x) {
            constexpr auto type = boost::hana::second(x);

            using member_type = std::decay_t<decltype(type(std::declval<entry<T>>()))>;
            using field_type  = underlying_type_t<member_type>;
            using insert_type = std::conditional_t<is_entry_v<field_type>, id_type, field_type>;

            auto call = boost::hana::find(type_to_sql_call, boost::hana::type_c<insert_type>).value();
            if constexpr (std::is_convertible_v<insert_type, std::string>)
            {
                return [call](auto res, auto idx) { return reinterpret_cast<const char *>(std::invoke(call, res, idx)); };
            }
            else
            {
                return call;
            }
        }));
    static constexpr auto sql_calls = drop_first(entry_sql_calls);

    static constexpr auto entry_sql_types = boost::hana::concat(
        boost::hana::make_tuple(BOOST_HANA_STRING("INTEGER PRIMARY KEY AUTOINCREMENT")), boost::hana::transform(accessors, [](auto x) {
            using namespace boost;
            using namespace hana::literals;

            constexpr auto type = hana::second(x);

            using member_type = std::decay_t<decltype(type(std::declval<entry_type>()))>;
            using field_type  = underlying_type_t<member_type>;

            // if field is a user defined type insert id instead of field
            using insert_type = std::conditional_t<is_entry_v<field_type>, id_type, field_type>;

            constexpr auto t   = hana::find(type_to_sql_type, hana::type_c<insert_type>);
            using has_optional = std::conditional_t<is_optional_v<member_type>, hana::string<>, decltype("NOT NULL"_s)>;
            return str::join<str::space_t>(hana::make_tuple(t.value(), has_optional{}));
        }));
    static constexpr auto sql_types = drop_first(entry_sql_types);

    static constexpr auto entry_zipped_fields = boost::hana::zip(entry_keys, entry_sql_types);
    static constexpr auto zipped_fields       = boost::hana::zip(keys, sql_types);

    static constexpr auto entry_fields = boost::hana::transform(entry_zipped_fields, [](auto x) { return str::join<str::space_t>(x); });
    static constexpr auto fields       = drop_first(entry_fields);
};

} // namespace lys::core::sql

#define NAME_AS_STR(name) #name

#define REGISTER_ENTRY(NAME, ...)                                          \
    BOOST_HANA_ADAPT_STRUCT(lys::core::sql::entry<NAME>, __VA_ARGS__, id); \
                                                                           \
    template <>                                                            \
    struct lys::core::sql::is_entry<NAME> : std::true_type                 \
    {};                                                                    \
                                                                           \
    template <>                                                            \
    constexpr auto lys::core::sql::entry_name<NAME>{BOOST_HANA_STRING(NAME_AS_STR(NAME))};
