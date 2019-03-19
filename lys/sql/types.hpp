#pragma once

#include <lys/sql/helpers.hpp>
#include <boost/hana/adapt_struct.hpp>
#include <boost/hana/drop_back.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/members.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/zip.hpp>
#include <fmt/format.h>
#include <sqlite3/sqlite3.h>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace lys::core::sql
{
using namespace boost::hana::literals;

constexpr auto convert_to_sqlite_type = boost::hana::make_map( //
    boost::hana::make_pair(boost::hana::type_c<std::string>, "TEXT"_s),
    boost::hana::make_pair(boost::hana::type_c<int>, "INT"_s),
    boost::hana::make_pair(boost::hana::type_c<double>, "REAL"_s));

constexpr auto convert_to_sqlite_function = boost::hana::make_map( //
    boost::hana::make_pair(boost::hana::type_c<std::string>, sqlite3_column_text),
    boost::hana::make_pair(boost::hana::type_c<int>, sqlite3_column_int),
    boost::hana::make_pair(boost::hana::type_c<double>, sqlite3_column_double));

} // namespace lys::core::sql

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
constexpr auto entry_name{""_s};

template <typename T>
struct is_optional : std::false_type
{};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type
{};

template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

template <typename T>
struct is_entry : std::false_type
{};

template <typename T>
constexpr bool is_entry_v = is_entry<T>::value;

template <typename T>
struct underlying_type
{
    using type = T;
};

template <typename T>
struct underlying_type<std::optional<T>>
{
    using type = T;
};

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
        boost::hana::make_tuple("id"_s), boost::hana::transform(accessors, [](auto x) { return boost::hana::first(x); }));
    static constexpr auto keys = drop_first(entry_keys);

    static constexpr auto entry_sql_calls =
        boost::hana::concat(boost::hana::make_tuple(sqlite3_column_int), boost::hana::transform(accessors, [](auto x) {
            constexpr auto type = boost::hana::second(x);

            using member_type = std::decay_t<decltype(type(std::declval<entry<T>>()))>;
            using field_type  = underlying_type_t<member_type>;
            using insert_type = std::conditional_t<is_entry_v<field_type>, id_type, field_type>;

            auto call = boost::hana::find(convert_to_sqlite_function, boost::hana::type_c<insert_type>).value();
            if constexpr (std::is_convertible_v<insert_type, std::string>)
            {
                return [call](auto res, auto idx) { return reinterpret_cast<const char *>(std::invoke(call, res, idx)); };
            }
            else
            {
                return call;
            }
            // return boost::hana::find(convert_to_sqlite_function, boost::hana::type_c<field_type>).value();
        }));
    static constexpr auto sql_calls = drop_first(entry_sql_calls);

    static constexpr auto entry_sql_types =
        boost::hana::concat(boost::hana::make_tuple("INTEGER PRIMARY KEY AUTOINCREMENT"_s), boost::hana::transform(accessors, [](auto x) {
            using namespace boost;
            using namespace hana::literals;

            constexpr auto type = hana::second(x);

            using member_type = std::decay_t<decltype(type(std::declval<entry_type>()))>;
            using field_type  = underlying_type_t<member_type>;

            // if field is a user defined type insert id instead of field
            using insert_type = std::conditional_t<is_entry_v<field_type>, id_type, field_type>;

            constexpr auto t   = hana::find(convert_to_sqlite_type, hana::type_c<insert_type>);
            using has_optional = std::conditional_t<is_optional_v<member_type>, hana::string<>, decltype("NOT NULL"_s)>;
            return helpers::join<helpers::space_t>(hana::make_tuple(t.value(), has_optional{}));
        }));
    static constexpr auto sql_types = drop_first(entry_sql_types);

    static constexpr auto entry_zipped_fields = boost::hana::zip(entry_keys, entry_sql_types);
    static constexpr auto zipped_fields       = boost::hana::zip(keys, sql_types);

    static constexpr auto entry_fields =
        boost::hana::transform(entry_zipped_fields, [](auto x) { return helpers::join<helpers::space_t>(x); });
    static constexpr auto fields = drop_first(entry_fields);
};

} // namespace lys::core::sql

namespace lys::core
{

struct brand_t
{
    std::string name;
};

struct model_t
{
    brand_t brand;
    std::string name;
};

struct factory_t
{
    std::string name;
};

struct car_t
{
    model_t model;
    double price;
    factory_t factory;
};

} // namespace lys::core

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

REGISTER_ENTRY(lys::core::brand_t, name);
REGISTER_ENTRY(lys::core::model_t, brand, name);
REGISTER_ENTRY(lys::core::factory_t, name);
REGISTER_ENTRY(lys::core::car_t, model, price, factory);
