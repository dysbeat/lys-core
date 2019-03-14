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
    constexpr auto accessors = hana::accessors<T>();

    auto fields = helpers::join<helpers::comma_sep_t>(hana::transform(accessors, [](auto x) {
        constexpr auto name = hana::first(x);
        constexpr auto type = hana::second(x);

        using member_type = std::decay_t<decltype(type(std::declval<T>()))>;
        using field_type  = underlying_type_t<member_type>;
        using insert_type = std::conditional_t<is_entry_v<field_type>, int, field_type>;

        constexpr auto t = hana::find(convert_to_sqlite_type, hana::type_c<insert_type>);
        return helpers::join<helpers::space_t>(hana::make_tuple(name, t.value()));
    }));

    constexpr auto query = helpers::format("CREATE TABLE \"_s\" (_);"_s, helpers::type_name<T>, fields);
    execute(db, query.c_str());
}

template <typename T>
void drop_table(sqlite3 * db)
{
    constexpr auto query = helpers::format("DROP TABLE \"_s\";"_s, helpers::type_name<T>);
    execute(db, query.c_str());
}

} // namespace lys::core::sql
