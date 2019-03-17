#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>
#include <lys/sql/traits.hpp>
#include <boost/hana/insert_range.hpp>

namespace lys::core::sql
{

template <typename T>
void create_table(sqlite3 * db)
{
    using namespace boost;
    using namespace hana::literals;

    using entry_type = entry<std::decay_t<T>>;

    constexpr auto accessors = hana::accessors<entry_type>();

    auto fields = helpers::join<helpers::comma_sep_t>(hana::transform(accessors, [](auto x) {
        constexpr auto name = hana::first(x);
        constexpr auto type = hana::second(x);

        using member_type = std::decay_t<decltype(type(std::declval<entry_type>()))>;
        using field_type  = underlying_type_t<member_type>;

        // if field is a user defined type insert id instead of field
        using insert_type = std::conditional_t<is_entry_v<field_type>, int, field_type>;

        constexpr auto t = hana::find(convert_to_sqlite_type, hana::type_c<insert_type>);
        if constexpr (is_optional_v<member_type>)
        {
            return helpers::join<helpers::space_t>(hana::make_tuple(name, t.value()));
        }
        else
        {
            return helpers::join<helpers::space_t>(hana::make_tuple(name, t.value(), "NOT NULL"_s));
        }
    }));

    constexpr auto query = helpers::format("CREATE TABLE \"_s\" (_);"_s, helpers::type_name<typename entry_type::base_type>, fields);
    execute(db, query.c_str());
}

template <typename T>
void drop_table(sqlite3 * db)
{
    using entry_type = entry<std::decay_t<T>>;

    constexpr auto query = helpers::format("DROP TABLE \"_s\";"_s, helpers::type_name<typename entry_type::base_type>);
    execute(db, query.c_str());
}

} // namespace lys::core::sql
