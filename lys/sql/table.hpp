#pragma once

#include <lys/sql/execute.hpp>
#include <lys/str/format.hpp>
#include <boost/hana/insert_range.hpp>

namespace lys::core::sql
{

template <typename T>
void create_table(sqlite3 * db)
{
    using namespace boost;
    using namespace hana::literals;

    using type_helper = entry_helper<T>;

    constexpr auto query =
        str::format<'$'>("CREATE TABLE \"$\" ($);"_s, type_helper::name, helpers::join<helpers::comma_sep_t>(type_helper::entry_fields));
    execute(db, query.c_str());
}

template <typename T>
void drop_table(sqlite3 * db)
{
    using namespace boost::hana::literals;

    using type_helper = entry_helper<T>;

    constexpr auto query = str::format<'$'>("DROP TABLE \"$\";"_s, type_helper::name);
    execute(db, query.c_str());
}

} // namespace lys::core::sql
