#pragma once

#include <lys/sql/execute.hpp>
#include <lys/str/format.hpp>
#include <lys/str/join.hpp>
#include <boost/hana/insert_range.hpp>
#include <lys/sql/entry.hpp>

namespace lys::core::sql
{

template <typename T>
void create_table(sqlite3 * db)
{
    using namespace boost;
    using namespace hana::literals;

    using type_helper = entry_helper<T>;

    constexpr auto query =
        str::format<'$'>("CREATE TABLE \"$\" ($);"_s, type_helper::name, str::join<str::comma_t>(type_helper::entry_fields));
    execute(db, query.c_str());
}

template <typename T>
void drop_table(sqlite3 * db)
{
    using namespace boost;
    using namespace hana::literals;

    using type_helper = entry_helper<T>;

    constexpr auto query = str::format<'$'>("DROP TABLE \"$\";"_s, type_helper::name);
    execute(db, query.c_str());
}

template <typename T>
bool table_exists(sqlite3 * db)
{
    using namespace boost;
    using namespace hana::literals;

    using type_helper = entry_helper<T>;

    constexpr auto query = str::format<'$'>("SELECT name FROM sqlite_master WHERE type='table' AND name='$'"_s, type_helper::name);

    sqlite3_stmt * res;
    prepare(db, query.c_str(), &res);

    return (sqlite3_step(res) == SQLITE_ROW);
}

} // namespace lys::core::sql
